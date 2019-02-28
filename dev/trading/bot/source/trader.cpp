#include "trader.hpp"

#include <ctime>
#include <iostream>

Trader::Trader(std::shared_ptr<ITradingPlatform> trading_platform)
    : trading_platform_(trading_platform),
      asset_pair_handler_(trading_platform) {
    std::cout << "Trader()\n";
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

    trading::Currency base_currency = asset_pair_handler_.SeparateBaseCurrency(asset_pair);

    //trading::Order order(0, 0, std::time(nullptr), asset_pair, order_type);
}
