#include "trading_platform_observer.hpp"

#include <iostream>

TradingPlatformObserver::TradingPlatformObserver(std::shared_ptr<ITradingPlatform> trading_platform)
    : trading_platform_(trading_platform) {

}

void TradingPlatformObserver::PeekEvents() {
    error::TradingError error_code = error::SUCCESS;

    error_code = PeekEvents(ClosedOrderTag);

    if(error::FAILED == error_code) {
        std::cout << "[TradingPlatformObserver] -> PeekEvents() -> Unable to peek closed orders" << std::endl;
    }

    error_code = PeekEvents(AssetPairTag);

    if(error::FAILED == error_code) {
        std::cout << "[TradingPlatformObserver] -> PeekEvents() -> Unable to peek asset pairs" << std::endl;
    }
}

void TradingPlatformObserver::DispatchEvents() {
    if(true == subscribers_.empty()) {
        return;
    }

    for(auto& item : closed_orders_) {
        bool is_cached = false;

        for(auto& cached_item : cached_closed_orders_) {

            if(cached_item == item) {
                is_cached = true;
                break;
            }

        }

        if(false == is_cached) {
            BroadcastEvent(item, ClosedOrderTag);
        }
    }

    for(auto& item : high_margin_asset_pairs_) {
        BroadcastEvent(item, AssetPairTag);
    }

    cached_closed_orders_ = closed_orders_;

    closed_orders_.clear();
    high_margin_asset_pairs_.clear();
}

void TradingPlatformObserver::SubsctibeObserver(std::shared_ptr<ITradingPlatformObserver> subscriber) {
    subscribers_.push_back(subscriber);
}

void TradingPlatformObserver::BroadcastEvent(const trading::id_t& order_ID, ClosedOrder) {
    for(auto& item : subscribers_) {
        item->NotifyOrderClosed(order_ID);
    }
}

void TradingPlatformObserver::BroadcastEvent(const trading::asset_pair_t& asset_pair, AssetPair) {
    for(auto& item : subscribers_) {
        item->NotifyPairFound(asset_pair);
    }
}

error::TradingError TradingPlatformObserver::PeekEvents(ClosedOrder) {
    error::TradingError error_code = error::SUCCESS;

    error_code = trading_platform_->GetClosedOrders(closed_orders_);

    return error_code;
}

error::TradingError TradingPlatformObserver::PeekEvents(AssetPair) {
    error::TradingError error_code = error::SUCCESS;

    if(true == all_asset_pairs_.empty()) {
        error_code = trading_platform_->GetAssetPairs(all_asset_pairs_);

        if(error::FAILED == error_code) {
            all_asset_pairs_.clear();
            return error_code;
        }
    }

    for(auto& pair : all_asset_pairs_) {
        trading::price_t margin = 0.0;

        error_code = trading_platform_->GetCurrecyMargin(pair, trading::kDefaultTimePeriod, margin);

        if(error::FAILED == error_code) {
            return error_code;
        }

        if(trading::kPairMarginPassRate <= margin) {
            high_margin_asset_pairs_.push_back(pair);
            std::cout << pair << " = " << margin << std::endl;
        }
    }

}
