#ifndef ASSET_PAIR_HANDLER_HPP_
#define ASSET_PAIR_HANDLER_HPP_

#include <list>
#include <memory>
#include <map>

#include "drawers.hpp"
#include "error_interface.hpp"
#include "trading_interface.hpp"

class AssetPairHandler {
  public:
    AssetPairHandler();

    error::TradingError AddAssetPair(const trading::common::asset_pair_t& asset_pair);
    error::TradingError RemovePair(const trading::common::asset_pair_t& asset_pair);
    trading::common::Currency SeparateBaseCurrency(const trading::common::asset_pair_t& asset_pair);
  private:
    error::TradingError IncreasePairsCounter(const trading::common::Currency& currency);
    void DecreasePairsCounter(const trading::common::Currency& currency);

    std::map<trading::common::Currency, int> pairs_count_;
    std::map<trading::common::Currency, int> max_pairs_count_;
    AssetPairsDrawer drawer_;
};

#endif // ASSET_PAIR_HANDLER_HPP_
