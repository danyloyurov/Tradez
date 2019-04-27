#ifndef TRADING_EVENT_HPP_
#define TRADING_EVENT_HPP_

#include "error_interface.hpp"
#include "trading_interface.hpp"

#include <vector>

class ITradingEvent {
  public:
    virtual error::TradingError GetMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) = 0;
    virtual error::TradingError GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) = 0;
    virtual error::TradingError GetClosedOrders(std::vector<trading::id_t>& closed_orders) = 0;
};

#endif // TRADING_EVENT_HPP_
