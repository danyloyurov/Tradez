#ifndef ASSET_PAIR_HANDLER_HPP_
#define ASSET_PAIR_HANDLER_HPP_

#include <vector>
#include <memory>
#include <map>

#include "trading_platform.hpp"
#include "trading_interface.hpp"

class AssetPairHandler {
    public:
        AssetPairHandler(std::shared_ptr<ITradingPlatform> trading_platform);

        error::TradingError AddAssetPair(const trading::pair_t& asset_pair);
        error::TradingError RemovePair(const trading::pair_t& asset_pair);
    private:
        trading::Currency SeparateBaseCurrency(const trading::pair_t& asset_pair);
        error::TradingError IncreasePairsCounter(const trading::Currency& currency);
        void DecreasePairsCounter(const trading::Currency& currency);

        std::map<trading::Currency, int> pairs_count_;
        std::map<trading::Currency, int> max_pairs_count_;
        std::vector<trading::pair_t> asset_pairs_;
        std::shared_ptr<ITradingPlatform> trading_platform_;
};

#endif // ASSET_PAIR_HANDLER_HPP_
