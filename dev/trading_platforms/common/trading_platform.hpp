#ifndef TRADING_PLATFORM_HPP_
#define TRADING_PLATFORM_HPP_

#include "error_interface.hpp"
#include "trading_interface.hpp"

#include <vector>

class ITradingPlatform {
  public:
    virtual ~ITradingPlatform() {}
    virtual error::TradingError PlaceOrder(trading::bot::Order& order) = 0;
    virtual error::TradingError RemoveOrder(const trading::common::id_t& order_ID) = 0;
    virtual error::TradingError GetAssetPairs(std::vector<trading::common::asset_pair_t>& asset_pairs) = 0;
    virtual error::TradingError GetAssetRawDump(const trading::common::asset_pair_t& pair) = 0;
    virtual error::TradingError GetClosedOrders(std::vector<trading::common::id_t>& closed_orders) = 0;
};

#endif // TRADING_PLATFORM_HPP_
