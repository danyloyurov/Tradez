#include "trading_platform_observer.hpp"

void TradingPlatformSubject::DispatchEvents() {
    if(true == subscribers_.empty()) {
        return;
    }

    for(auto& item : closed_orders_) {
        BroadcastEvent(item, ClosedOrderTag);
    }

    for(auto& item : available_asset_pairs_) {
        BroadcastEvent(item, PairFoundTag);
    }

    closed_orders_.clear();
    available_asset_pairs_.clear();
}

void TradingPlatformSubject::SubsctibeObserver(std::unique_ptr<ITradingPlatformObserver> subscriber) {
    subscribers_.push_back(std::move(subscriber));
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
