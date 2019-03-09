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

void Trader::PollOrders() {
    if(false == failed_sell_orders_.empty()) {
        error::TradingError error_code = error::SUCCESS;

        for(auto& order : failed_sell_orders_) {
            error_code = orders_handler_.PlaceSellOrder(order);

            if(error::SUCCESS == error_code) {
                failed_sell_orders_.remove(order);
            }

            std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
        }
    }

    std::vector<trading::asset_pair_t> expired_pairs = orders_handler_.PollExpiredOrders();

    for(auto& pair : expired_pairs) {
        asset_pair_handler_.RemovePair(pair);
    }
}

void Trader::NotifyOrderClosed(const trading::id_t& order_ID) {
    std::cout << "[Trader] NotifyOrderClosed -> " << order_ID << std::endl;

    error::TradingError error_code = error::SUCCESS;

    error_code = orders_handler_.PlaceSellOrder(order_ID);

    if(error::FAILED == error_code) {
        std::cout << "[Trader::Error] NotifyPairFound -> Unable to add sell order" << std::endl;
        failed_sell_orders_.push_back(order_ID);
    }

    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}

void Trader::NotifyPairFound(const trading::asset_pair_t& asset_pair) {
    std::cout << "[Trader] NotifyPairFound -> " << asset_pair << std::endl;

    error::TradingError error_code = error::SUCCESS;

    error_code = asset_pair_handler_.AddAssetPair(asset_pair);

    if(error::FAILED == error_code) {
        std::cout << "[Trader::Error] NotifyPairFound -> Unable to add pair" << std::endl;
        return;
    }

    error_code = orders_handler_.PlaceBuyOrder(asset_pair, asset_pair_handler_.SeparateBaseCurrency(asset_pair));

    if(error::FAILED == error_code) {
        std::cout << "[Trader::Error] NotifyPairFound -> Unable to add buy order" << std::endl;
        asset_pair_handler_.RemovePair(asset_pair);
    }

    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}
