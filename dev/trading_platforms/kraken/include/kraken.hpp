#ifndef TRADING_PLATFORM_KRAKEN_HPP_
#define TRADING_PLATFORM_KRAKEN_HPP_

#include "trading_platform.hpp"
#include "kclient.hpp"

class KrakenPlatform : public ITradingPlatform {
  public:
    KrakenPlatform(const std::string& public_key, const std::string& private_key);
    ~KrakenPlatform();

    virtual error::TradingError PlaceOrder(trading::Order& order) final;
    virtual error::TradingError RemoveOrder(const trading::id_t& order_id) final;
    virtual error::TradingError GetAssetRawDump(const trading::common::asset_pair_t& asset_pair,
                                                std::vector<trading::analyzer::RawAsset>& raw_asset_dump) final;
    virtual error::TradingError GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) final;
    virtual error::TradingError GetClosedOrders(std::vector<trading::id_t>& closed_orders) final;
  private:
    Kraken::KClient kraken_client_;
};

#endif // TRADING_PLATFORM_KRAKEN_HPP_
