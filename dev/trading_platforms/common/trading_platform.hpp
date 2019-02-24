#ifndef TRADING_PLATFORM_HPP_
#define TRADING_PLATFORM_HPP_

#include "error_interface.hpp"
#include "trading_interface.hpp"

#include <vector>

class ITradingPlatform {
    public:
        virtual error::TradingError PlaceOrder(trading::Order& order, const trading::PricePresset& presset) = 0;
        virtual error::TradingError RemoveOrder(const trading::id_t& order_ID) = 0;
        virtual error::TradingError GetAssetPairs(std::vector<trading::pair_t>& asset_pairs) = 0;
        virtual error::TradingError GetCurrecyMargin(const trading::pair_t& pair, const int& time_period, trading::price_t& currency_margin) = 0;
        virtual error::TradingError GetCurrecyPrice(const trading::pair_t& pair, const int& time_period, trading::price_t& price) = 0;
        virtual error::TradingError GetPairPriceFormat(const trading::pair_t& pair, trading::PricePresset& presset) = 0;
        virtual error::TradingError GetVolumeToBuy(const trading::pair_t& pair, const trading::price_t& fiat_volume, trading::volume_t& crypto_volume) = 0;
        virtual error::TradingError GetAccountBalance(const trading::currecy_t& currency, trading::volume_t& value) = 0;
        virtual error::TradingError GetClosedOrders(const std::vector<trading::id_t>& orders, std::vector<trading::id_t>& closed_orders) = 0;
        virtual error::TradingError GetOrderVolume(const trading::id_t& id, trading::volume_t& remaining_volume) = 0;
};

#endif // TRADING_PLATFORM_HPP_
