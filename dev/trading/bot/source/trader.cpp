#include "trader.hpp"

#include <ctime>
#include <iostream>

Trader::Trader(std::shared_ptr<ITradingPlatform> trading_platform)
    : trading_platform_(trading_platform),
      asset_pair_handler_(trading_platform) {
    std::cout << "Trader()\n";

    base_currency_volumes_[trading::USD] = trading::kUSDBaseBuyVolume;
    base_currency_volumes_[trading::EUR] = trading::kEURBaseBuyVolume;
    base_currency_volumes_[trading::XBT] = trading::kXBTBaseBuyVolume;
}

Trader::~Trader() {
    std::cout << "~Trader()\n";
}

void Trader::NotifyOrderClosed(const trading::id_t& order_ID) {
    std::cout << "[Trader] NotifyOrderClosed -> " << order_ID << std::endl;
}

void Trader::NotifyPairFound(const trading::asset_pair_t& asset_pair) {
    std::cout << "[Trader] NotifyPairFound -> " << asset_pair << std::endl;

    error::TradingError error_code = error::SUCCESS;

    error_code = asset_pair_handler_.AddAssetPair(asset_pair);

    if(error::FAILED == error_code) {
        return;
    }

    PlaceOrder(asset_pair, trading::BUY);
}

void Trader::PlaceOrder(const trading::asset_pair_t& asset_pair, const trading::OrderType& order_type) {
    std::cout << "[Trader] PlaceOrder -> " << asset_pair << std::endl;

    error::TradingError error_code = error::SUCCESS;

    trading::Currency base_currency = asset_pair_handler_.SeparateBaseCurrency(asset_pair);
    trading::volume_t base_currency_volume = base_currency_volumes_[base_currency];
    trading::volume_t order_volume = 0.0;
    trading::price_t price = 0.0;
    trading::PricePresset price_presset(0,0);

    error_code = trading_platform_->GetPairPriceFormat(asset_pair, price_presset);

    if(error::FAILED == error_code) {
        std::cout << "[Trader] PlaceOrder::Error -> Unable to get pair price presset" << std::endl;
        return;
    }

    if(trading::BUY == order_type) {
        error_code = trading_platform_->GetCurrecyPrice(asset_pair, trading::kDefaultTimePeriod, price);

        if(error::FAILED == error_code) {
            std::cout << "[Trader] PlaceOrder::Error -> Unable to get pair price" << std::endl;
            return;
        }

        error_code = trading_platform_->GetVolumeToBuy(asset_pair, base_currency_volume, order_volume);

        if(error::FAILED == error_code) {
            std::cout << "[Trader] PlaceOrder::Error -> Unable to get pair volume" << std::endl;
            return;
        }
    } else {
        //TODO: Implement closed orders replacing
    }

    price = trading::price_calculator(price, order_type);

    trading::Order order(asset_pair, order_volume, price, std::time(nullptr), order_type);

    error_code = trading_platform_->PlaceOrder(order, price_presset);

    if(error::FAILED == error_code) {
        std::cout << "[Trader] PlaceOrder::Error -> Unable to place order" << std::endl;
        return;
    }

    open_orders_.push_back(order);
}
