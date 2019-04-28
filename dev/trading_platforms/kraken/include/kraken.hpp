#ifndef TRADING_PLATFORM_KRAKEN_HPP_
#define TRADING_PLATFORM_KRAKEN_HPP_

#include "trading_platform.hpp"
#include "kclient.hpp"

class KrakenPlatform : public ITradingPlatform {
  public:
    KrakenPlatform(const std::string& public_key, const std::string& private_key);
    ~KrakenPlatform();

    virtual error::TradingError PlaceOrder(trading::Order& order) final;
    virtual error::TradingError RemoveOrder(const trading::id_t& order_ID) final;
    virtual error::TradingError GetAveragePrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) final;
    virtual error::TradingError GetHighestPrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) final;
    virtual error::TradingError GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresset& presset) final;
    virtual error::TradingError GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::volume_t& base_volume, trading::volume_t& crypto_volume) final;
    virtual error::TradingError GetMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) final;
    virtual error::TradingError GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) final;
    virtual error::TradingError GetClosedOrders(std::vector<trading::id_t>& closed_orders) final;
  private:
    Kraken::KClient kraken_client_;
    std::vector<Kraken::KTrade> pair_dump_;
};

#endif // TRADING_PLATFORM_KRAKEN_HPP_
