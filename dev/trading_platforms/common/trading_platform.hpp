#ifndef TRADING_PLATFORM_HPP_
#define TRADING_PLATFORM_HPP_

#include "error_interface.hpp"
#include "trading_interface.hpp"

#include <vector>

class ITradingPlatform {
    public:
        virtual error::TradingError PlaceOrder(trading::Order& order) = 0;
        virtual error::TradingError RemoveOrder(const trading::id_t& order_ID) = 0;
        virtual error::TradingError GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) = 0;
        virtual error::TradingError GetCurrecyMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) = 0;
        virtual error::TradingError GetCurrecyPrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) = 0;
        virtual error::TradingError GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresset& presset) = 0;
        virtual error::TradingError GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::volume_t& base_volume, trading::volume_t& crypto_volume) = 0;
        virtual error::TradingError GetClosedOrders(std::vector<trading::id_t>& closed_orders) = 0;
};

#endif // TRADING_PLATFORM_HPP_
