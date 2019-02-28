#include "asset_pair_handler.hpp"
#include "container_helper.hpp"
#include "multipurpose_converter.hpp"

#include <iostream>

AssetPairHandler::AssetPairHandler(std::shared_ptr<ITradingPlatform> trading_platform)
    : trading_platform_(trading_platform) {
    pairs_count_[trading::USD] = 0;
    pairs_count_[trading::EUR] = 0;
    pairs_count_[trading::XBT] = 0;

    max_pairs_count_[trading::USD] = trading::kMaxUSDPairsCount;
    max_pairs_count_[trading::EUR] = trading::kMaxEURPairsCount;
    max_pairs_count_[trading::XBT] = trading::kMaxXBTPairsCount;
}

error::TradingError AssetPairHandler::AddAssetPair(const trading::asset_pair_t& asset_pair) {
    std::vector<trading::asset_pair_t>::const_iterator element = Searcher<trading::asset_pair_t>::Search(asset_pairs_, asset_pair);

    if(asset_pairs_.end() != element) {
        std::cout << "Already added pair = " << asset_pair << std::endl;
        return error::FAILED;
    }

    error::TradingError error_code = error::SUCCESS;
    trading::Currency base_currency = trading::PASS;

    base_currency = SeparateBaseCurrency(asset_pair);
    error_code = IncreasePairsCounter(base_currency);

    if(error::FAILED == error_code) {
        return error_code;
    }

    std::cout << "base_currency = " << base_currency << std::endl
              << "asset_pair = " << asset_pair << std::endl
              << "pairs_count_[base_currency] = " << pairs_count_[base_currency] << std::endl;

    asset_pairs_.push_back(asset_pair);
    asset_pairs_ = Sorter<trading::asset_pair_t>::Sort(asset_pairs_);
}

error::TradingError AssetPairHandler::RemovePair(const trading::asset_pair_t& asset_pair) {
    std::vector<trading::asset_pair_t>::const_iterator pair_to_remove = Searcher<trading::asset_pair_t>::Search(asset_pairs_, asset_pair);
    std::vector<trading::asset_pair_t> updated_asset_pairs;

    std::vector<trading::asset_pair_t>::const_iterator iter = asset_pairs_.begin();
    for( ; iter < pair_to_remove; iter++) {
        updated_asset_pairs.push_back(*iter);
    }

    iter = pair_to_remove + 1;
    for( ; iter != asset_pairs_.end(); iter++) {
        updated_asset_pairs.push_back(*iter);
    }

    asset_pairs_ = updated_asset_pairs;
}

trading::Currency AssetPairHandler::SeparateBaseCurrency(const trading::asset_pair_t& asset_pair) {
    for(auto& item : pairs_count_) {
        std::string string_currency = MultipurposeConverter::ConvertCurrencyToString(item.first);

        if(std::string::npos != asset_pair.find(string_currency) && 0 < asset_pair.find(string_currency)) {
            return item.first;
        }
    }

    return trading::PASS;    
}

error::TradingError AssetPairHandler::IncreasePairsCounter(const trading::Currency& currency) {
    error::TradingError error_code = error::SUCCESS;

    if(max_pairs_count_[currency] > pairs_count_[currency]) {
        pairs_count_[currency]++;
    } else {
        error_code = error::FAILED;
    }

    return error_code;
}

void AssetPairHandler::DecreasePairsCounter(const trading::Currency& currency) {
    if(pairs_count_[currency] > 0) {
        pairs_count_[currency]--;
    }
}
