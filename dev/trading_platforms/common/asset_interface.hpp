#ifndef ASSET_INTERFACE_HPP_
#define ASSET_INTERFACE_HPP_

#include "error_interface.hpp"
#include "trading_interface.hpp"

class IAsset {
  public:
    virtual error::TradingError PlaceOrder(trading::Order& order) = 0;
    virtual error::TradingError RemoveOrder(const trading::id_t& order_ID) = 0;
    virtual error::TradingError GetAveragePrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) = 0;
    virtual error::TradingError GetHighestPrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) = 0;
    virtual error::TradingError GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresset& presset) = 0;
    virtual error::TradingError GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::volume_t& base_volume, trading::volume_t& crypto_volume) = 0;
};

#endif // ASSET_INTERFACE_HPP_
