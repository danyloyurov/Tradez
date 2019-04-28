#include "orders_handler.hpp"
#include "logger.hpp"

#include <iostream>

OrdersHandler::OrdersHandler(std::shared_ptr<ITradingPlatform> trading_platform)
  : cached_order_("", 0, 0, 0, trading::BUY, trading::PricePresset(0, 0)),
    trading_platform_(trading_platform) {
  base_currency_volumes_[trading::USD] = trading::kUSDBaseBuyVolume;
  base_currency_volumes_[trading::EUR] = trading::kEURBaseBuyVolume;
  base_currency_volumes_[trading::XBT] = trading::kXBTBaseBuyVolume;
}

OrdersHandler::~OrdersHandler() {

}

trading::Order OrdersHandler::GetCachedOrder() {
  Logger::Instanse().Log("[OrderHandler] GetCachedOrder", Logger::FileTag);
  trading::Order cached = cached_order_;
  cached_order_ = trading::Order("", 0, 0, 0, trading::BUY, trading::PricePresset(0, 0));

  return cached;
}

std::vector<trading::asset_pair_t> OrdersHandler::PollExpiredOrders() {
  Logger::Instanse().Log("[OrderHandler] PollExpiredOrders", Logger::FileTag);

  error::TradingError error_code = error::SUCCESS;
  time_t current_time = time(NULL);
  std::vector<trading::asset_pair_t> expired_pairs;

  Logger::Instanse().Log("[OrderHandler::Debug] open_orders.size(): " + std::to_string(open_orders_.size()), Logger::FileTag);

  for(auto& order : open_orders_) {

    if(trading::SELL == order.type_) {
      Logger::Instanse().Log("[OrderHandler] Sell order can't expire", Logger::FileTag);
      continue;
    }

    Logger::Instanse().Log("[OrderHandler] Time diff: " + std::to_string(current_time - order.time_placed_), Logger::FileTag);

    if(current_time - order.time_placed_ > trading::kOrderExpirationTimePeriod) {
      error_code = RemoveOrder(order.trading_patform_ID_, RemoteOrderTag);

      if(error::SUCCESS == error_code) {
        Logger::Instanse().Log("[OrderHandler] PollExpiredOrders -> " + order.asset_pair_, Logger::FileTag);
        expired_pairs.push_back(order.asset_pair_);
        RemoveOrder(order.trading_patform_ID_, LocalOrderTag);
      }

    }

  }

  return expired_pairs;
}

error::TradingError OrdersHandler::PlaceBuyOrder(const trading::asset_pair_t& asset_pair, const trading::Currency& base_currency) {
  Logger::Instanse().Log("[OrderHandler] PlaceBuyOrder -> " + asset_pair, Logger::FileTag);

  error::TradingError error_code = error::SUCCESS;

  trading::volume_t base_currency_volume = base_currency_volumes_[base_currency];
  trading::volume_t order_volume = 0.0;
  trading::price_t price = 0.0;
  trading::PricePresset price_presset(0,0);

  error_code = trading_platform_->GetPairPriceFormat(asset_pair, price_presset);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[OrderHandler::Error] Unable to get pair price presset", Logger::FileTag);
    return error_code;
  }

  error_code = trading_platform_->GetAveragePrice(asset_pair, trading::kDefaultTimePeriod, price);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[OrderHandler::Error] Unable to get pair price", Logger::FileTag);
    return error_code;
  }

  error_code = trading_platform_->GetVolumeToBuy(asset_pair, base_currency_volume, order_volume);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[OrderHandler::Error] Unable to get pair volume", Logger::FileTag);
    return error_code;
  }

  price = trading::price_calculator(price, trading::BUY);

  trading::Order buy_order(asset_pair, order_volume, price, time(NULL), trading::BUY, price_presset);

  error_code = trading_platform_->PlaceOrder(buy_order);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[OrderHandler::Error] Unable to place buy order", Logger::FileTag);
    return error_code;
  }

  open_orders_.push_back(buy_order);
  open_orders_ = OrdersSorter::Sort(open_orders_);

  drawer_.Flush();
  drawer_.Draw(open_orders_);
  Logger::Instanse().Log("[OrderHandler::Debug] open_orders_.size(): " + std::to_string(open_orders_.size()), Logger::FileTag);

  return error_code;
}

error::TradingError OrdersHandler::PlaceSellOrder(const trading::id_t& order_ID) {
  Logger::Instanse().Log("[OrderHandler] PlaceSellOrder -> " + order_ID, Logger::FileTag);

  typename OrdersVector::const_iterator order_iterator = OrdersSearcher::Search(open_orders_, trading::Order(order_ID));

  if(open_orders_.end() == order_iterator) {
    Logger::Instanse().Log("[OrderHandler::Error] Can't find order! -> " + order_ID, Logger::FileTag);
    return error::SUCCESS;
  }

  if(trading::SELL == order_iterator->type_) {
    Logger::Instanse().Log("[OrderHandler] pair cycle ended! Removing pair -> " + order_iterator->asset_pair_, Logger::FileTag);

    cached_order_ = *order_iterator;
    RemoveOrder(order_ID, LocalOrderTag);
    open_orders_ = OrdersSorter::Sort(open_orders_);
    return error::SUCCESS;
  }

  error::TradingError error_code = error::SUCCESS;

  trading::price_t sell_price = trading::price_calculator(order_iterator->price_, trading::SELL);
  trading::volume_t sell_volume = trading::fee_calculator(order_iterator->volume_, trading::kKrakenFee);

  trading::Order sell_order(order_iterator->asset_pair_, sell_volume, sell_price, time(NULL), trading::SELL, order_iterator->price_presset_);

  error_code = trading_platform_->PlaceOrder(sell_order);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[OrderHandler::Error] Unable to place sell order", Logger::FileTag);
    return error_code;
  }

  RemoveOrder(order_ID, LocalOrderTag);
  open_orders_.push_back(sell_order);
  open_orders_ = OrdersSorter::Sort(open_orders_);

  drawer_.Flush();
  drawer_.Draw(open_orders_);
  Logger::Instanse().Log("[OrderHandler::Debug] open_orders_.size(): " + std::to_string(open_orders_.size()), Logger::FileTag);

  return error_code;
}

error::TradingError OrdersHandler::RemoveOrder(const trading::id_t& order_ID, OrdersHandler::LocalOrder) {
  Logger::Instanse().Log("[OrderHandler] RemoveOrder(LocalOrder) -> " + order_ID, Logger::FileTag);

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

  drawer_.Flush();
  drawer_.Draw(open_orders_);
  Logger::Instanse().Log("[OrderHandler::Debug] open_orders_.size(): " + std::to_string(open_orders_.size()), Logger::FileTag);

  return error::SUCCESS;
}

error::TradingError OrdersHandler::RemoveOrder(const trading::id_t& order_ID, OrdersHandler::RemoteOrder) {
  Logger::Instanse().Log("[OrderHandler] RemoveOrder(RemoteOrder) -> " + order_ID, Logger:: FileTag);

  error::TradingError error_code = trading_platform_->RemoveOrder(order_ID);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[OrderHandler::Error] Unable to remove remote order", Logger:: FileTag);
  }

  return error_code;
}

