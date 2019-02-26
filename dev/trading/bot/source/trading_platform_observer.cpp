#include "trading_platform_observer.hpp"

#include <iostream>

TradingPlatformSubject::TradingPlatformSubject(std::shared_ptr<ITradingPlatform> trading_platform)
    : trading_platform_(trading_platform) {

}

void TradingPlatformSubject::PeekEvents() {
    error::TradingError error_code = error::SUCCESS;

    error_code = trading_platform_->GetClosedOrders(closed_orders_);

    if(error::FAILED == error_code) {
        std::cout << "[TradingPlatformSubject] -> PeekEvents() -> Unable to peek closed orders" << std::endl;
    }

    error_code = trading_platform_->GetAssetPairs(available_asset_pairs_);

    if(error::FAILED == error_code) {
        std::cout << "[TradingPlatformSubject] -> PeekEvents() -> Unable to peek asset pairs" << std::endl;
    }
}

void TradingPlatformSubject::DispatchEvents() {
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

    for(auto& item : available_asset_pairs_) {
        BroadcastEvent(item, PairFoundTag);
    }

    cached_closed_orders_ = closed_orders_;

    closed_orders_.clear();
    available_asset_pairs_.clear();
}

void TradingPlatformSubject::SubsctibeObserver(std::shared_ptr<ITradingPlatformObserver> subscriber) {
    subscribers_.push_back(subscriber);
}

void TradingPlatformSubject::BroadcastEvent(const trading::id_t& order_ID, ClosedOrder) {
    for(auto& item : subscribers_) {
        item->NotifyOrderClosed(order_ID);
    }
}

void TradingPlatformSubject::BroadcastEvent(const trading::pair_t& asset_pair, PairFound) {
    for(auto& item : subscribers_) {
        item->NotifyPairFound(asset_pair);
    }
}
