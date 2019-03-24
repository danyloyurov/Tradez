#ifndef TRADING_PLATFORM_BITTREX_HPP_
#define TRADING_PLATFORM_BITTREX_HPP_

#include "trading_platform.hpp"
#include "external_lib/libbittrex/client.h"

class BittrexPlatform : public ITradingPlatform {
    public:
        BittrexPlatform(const std::string& public_key, const std::string& private_key);
        ~BittrexPlatform();

        virtual error::TradingError PlaceOrder(trading::Order& order) final;
        virtual error::TradingError RemoveOrder(const trading::id_t& order_ID) final;
        virtual error::TradingError GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) final;
        virtual error::TradingError GetCurrecyMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) final;
        virtual error::TradingError GetCurrecyPrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) final;
        virtual error::TradingError GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresset& presset) final;
        virtual error::TradingError GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::price_t& base_volume, trading::volume_t& crypto_volume) final;
        virtual error::TradingError GetClosedOrders(std::vector<trading::id_t>& closed_orders) final;
    private:
        bittrex::Client btx_client_;
};

#endif // TRADING_PLATFORM_BITTREX_HPP_
