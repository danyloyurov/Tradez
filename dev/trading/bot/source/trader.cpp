#include "trader.hpp"

#include <thread>
#include <chrono>
#include <iostream>

Trader::Trader(std::shared_ptr<ITradingPlatform> trading_platform)
    : asset_pair_handler_(trading_platform),
      orders_handler_(trading_platform) {

}

Trader::~Trader() {

}

void Trader::NotifyOrderClosed(const trading::id_t& order_ID) {
    std::cout << "[Trader] NotifyOrderClosed -> " << order_ID << std::endl;

    orders_handler_.PlaceSellOrder(order_ID);
    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}

void Trader::NotifyPairFound(const trading::asset_pair_t& asset_pair) {
    std::cout << "[Trader] NotifyPairFound -> " << asset_pair << std::endl;

    error::TradingError error_code = error::SUCCESS;

    error_code = asset_pair_handler_.AddAssetPair(asset_pair);

    if(error::FAILED == error_code) {
        return;
    }

    orders_handler_.PlaceBuyOrder(asset_pair, asset_pair_handler_.SeparateBaseCurrency(asset_pair));
    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}
