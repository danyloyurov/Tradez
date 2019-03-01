#include "trader.hpp"

#include <thread>
#include <chrono>
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

    PlaceSellOrder(order_ID);
    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}

void Trader::NotifyPairFound(const trading::asset_pair_t& asset_pair) {
    std::cout << "[Trader] NotifyPairFound -> " << asset_pair << std::endl;

    error::TradingError error_code = error::SUCCESS;

    error_code = asset_pair_handler_.AddAssetPair(asset_pair);

    if(error::FAILED == error_code) {
        return;
    }

    PlaceBuyOrder(asset_pair);
    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}

void Trader::PlaceBuyOrder(const trading::asset_pair_t& asset_pair) {
    std::cout << "[Trader] PlaceBuyOrder -> " << asset_pair << std::endl;

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

    price = trading::price_calculator(price, trading::BUY);

    trading::Order buy_order(asset_pair, order_volume, price, trading::BUY, price_presset);

    error_code = trading_platform_->PlaceOrder(buy_order);

    if(error::FAILED == error_code) {
        std::cout << "[Trader] PlaceBuyOrder::Error -> Unable to place buy order" << std::endl;
        return;
    }

    open_orders_.push_back(buy_order);
    open_orders_ = OrdersSorter::Sort(open_orders_);
}

void Trader::PlaceSellOrder(const trading::id_t& order_ID) {
    std::cout << "[Trader] PlaceSellOrder -> " << order_ID << std::endl;

    typename OrdersVector::const_iterator order_iterator = OrdersSearcher::Search(open_orders_, trading::Order(order_ID));

    error::TradingError error_code = error::SUCCESS;

    trading::price_t sell_price = 0.0;
    trading::PricePresset price_presset(0, 0);
    sell_price = trading::price_calculator(order_iterator->price_, trading::SELL);

    trading::Order sell_order(order_iterator->asset_pair_, order_iterator->volume_, sell_price, trading::SELL, order_iterator->price_presset_);

    error_code = trading_platform_->PlaceOrder(sell_order);

    if(error::FAILED == error_code) {
        std::cout << "[Trader] PlaceSellOrder::Error -> Unable to place sell order" << std::endl;
        return;
    }

    RemoveOrder(order_ID);
    open_orders_.push_back(sell_order);
    open_orders_ = OrdersSorter::Sort(open_orders_);
}

void Trader::RemoveOrder(const trading::id_t& order_ID) {
    std::cout << "[Trader] RemoveOrder -> " << order_ID << std::endl;

    typename OrdersVector::const_iterator order_to_remove = OrdersSearcher::Search(open_orders_, trading::Order(order_ID));
    std::vector<trading::Order> open_orders;
    std::vector<trading::Order>::iterator order_iterator = open_orders_.begin();

    for( ; order_iterator < order_to_remove; order_iterator++) {
        open_orders.push_back(*order_iterator);
    }

    order_iterator++;

    for( ; order_iterator < open_orders_.end(); order_iterator++) {
        open_orders.push_back(*order_iterator);
    }

    open_orders_ = open_orders;
}
