#include "trader.hpp"
#include "logger.hpp"

#include <thread>
#include <chrono>
#include <iostream>

Trader::Trader(std::shared_ptr<ITradingPlatform> trading_platform)
    : asset_pair_handler_(trading_platform),
      orders_handler_(trading_platform) {

}

Trader::~Trader() {

}

void Trader::PollFailedOrders() {
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
}

void Trader::NotifyOrderClosed(const trading::id_t& order_ID) {
    Logger::Instanse().Log("[Trader] NotifyOrderClosed -> " + order_ID, Logger::FileTag);

    error::TradingError error_code = error::SUCCESS;

    error_code = orders_handler_.PlaceSellOrder(order_ID);

    if(error::FAILED == error_code) {
        Logger::Instanse().Log("[Trader::Error]  Unable to add sell order", Logger::FileTag);
        failed_sell_orders_.push_back(order_ID);
    } else {
        trading::Order closed_order = orders_handler_.GetCachedOrder();

        if(trading::SELL == closed_order.type_) {
            Logger::Instanse().Log("[Trader] NotifyOrderClosed -> " + order_ID + " ending cycle", Logger::FileTag);
            asset_pair_handler_.RemovePair(closed_order.asset_pair_);
        }

    }

    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}

void Trader::NotifyPairFound(const trading::asset_pair_t& asset_pair) {
    Logger::Instanse().Log("[Trader] NotifyPairFound -> " + asset_pair, Logger::FileTag);

    error::TradingError error_code = error::SUCCESS;

    error_code = asset_pair_handler_.AddAssetPair(asset_pair);

    if(error::FAILED == error_code) {
        Logger::Instanse().Log("[Trader::Error] Unable to add pair", Logger::FileTag);
        return;
    }

    error_code = orders_handler_.PlaceBuyOrder(asset_pair, asset_pair_handler_.SeparateBaseCurrency(asset_pair));

    if(error::FAILED == error_code) {
        Logger::Instanse().Log("[Trader::Error] Unable to add buy order", Logger::FileTag);
        asset_pair_handler_.RemovePair(asset_pair);
    }

    std::this_thread::sleep_for(std::chrono::seconds(trading::kPrivateRequestSleep));
}
