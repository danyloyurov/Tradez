#include "trading_platform_observer.hpp"
#include "logger.hpp"

#include <thread>
#include <chrono>
#include <iostream>

TradingPlatformObserver::TradingPlatformObserver(std::shared_ptr<ITradingPlatform> trading_platform)
  : trading_platform_(trading_platform) {
}

void TradingPlatformObserver::PeekEvents() {
  error::TradingError error_code = error::SUCCESS;

  error_code = PeekEvents(ClosedOrderTag);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[TradingPlatformObserver::Error] -> Unable to peek closed orders", Logger::FileTag);
  }

  error_code = PeekEvents(AssetPairTag);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[TradingPlatformObserver::Error] -> Unable to peek asset pairs margins", Logger::FileTag);
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

void TradingPlatformObserver::BroadcastEvent(const trading::common::id_t& order_ID, ClosedOrder) {
  for(auto& item : subscribers_) {
    item->NotifyOrderClosed(order_ID);
  }
}

void TradingPlatformObserver::BroadcastEvent(const trading::common::asset_pair_t& asset_pair, AssetPair) {
  for(auto& item : subscribers_) {
    item->NotifyPairFound(asset_pair);
  }
}

error::TradingError TradingPlatformObserver::PeekEvents(ClosedOrder) {
  error::TradingError error_code = error::SUCCESS;

  error_code = trading_platform_->GetClosedOrders(closed_orders_);

  if(true == cached_closed_orders_.empty()) {
    cached_closed_orders_ = closed_orders_;
  }

  return error_code;
}

error::TradingError TradingPlatformObserver::PeekEvents(AssetPair) {
  error::TradingError error_code = error::SUCCESS;

  if(true == all_asset_pairs_.empty()) {
    error_code = trading_platform_->GetAssetPairs(all_asset_pairs_);

    if(error::FAILED == error_code) {
      all_asset_pairs_.clear();
      Logger::Instanse().Log("[TradingPlatformObserver::Error] -> Unable to init asset pairs", Logger::FileTag);
      return error_code;
    }
  }

  for(auto& pair : all_asset_pairs_) {
    trading::common::price_t margin = 0.0;

    error_code = trading_platform_->GetMargin(pair, trading::kDefaultTimePeriod, margin);

    if(error::FAILED == error_code) {
      Logger::Instanse().Log("[TradingPlatformObserver::Error] -> Unable to get " + pair + " margin", Logger::FileTag);
      return error_code;
    }

    if(trading::kPairMarginPassRate <= margin) {
      trading::common::price_t median_price = 0.0;
      trading::common::price_t highest_price = 0.0;

      error_code = trading_platform_->GetAveragePrice(pair, trading::kDefaultTimePeriod, median_price);

      if(error::FAILED == error_code) {
        Logger::Instanse().Log("[TradingPlatformObserver::Error] Unable to get pair price", Logger::FileTag);
        return error_code;
      }

      error_code = trading_platform_->GetHighestPrice(pair, trading::kRiskTimePeriod, highest_price);

      if(error::FAILED == error_code) {
        Logger::Instanse().Log("[TradingPlatformObserver::Error] Unable to get highest pair price", Logger::FileTag);
        return error_code;
      }

      trading::price_t price_delta = (highest_price - median_price) /  (highest_price / 100);

      if(trading::kFailingTrend <= price_delta) {
        Logger::Instanse().Log("[TradingPlatformObserver] -> " + pair + " failing. Price delta: " + std::to_string(price_delta), Logger::FileTag);
        continue;
      }

      high_margin_asset_pairs_.push_back(pair);
      Logger::Instanse().Log("[TradingPlatformObserver] -> " + pair +
                             " margin: " + std::to_string(margin) +
                             " price delta: " + std::to_string(price_delta), Logger::FileTag);
    }

    std::this_thread::sleep_for(std::chrono::seconds(trading::kPublicRequestSleep));
  }

  return error_code;
}

