#include "asset_pair_handler.hpp"
#include "container_helper.hpp"
#include "multipurpose_converter.hpp"
#include "logger.hpp"

#include <iostream>

AssetPairHandler::AssetPairHandler() {
  pairs_count_[trading::common::USD] = 0;
  pairs_count_[trading::common::EUR] = 0;
  pairs_count_[trading::common::XBT] = 0;

  max_pairs_count_[trading::common::USD] = trading::bot::kMaxUSDPairsCount;
  max_pairs_count_[trading::common::EUR] = trading::bot::kMaxEURPairsCount;
  max_pairs_count_[trading::common::XBT] = trading::bot::kMaxXBTPairsCount;
}

error::TradingError AssetPairHandler::AddAssetPair(const trading::common::asset_pair_t& asset_pair) {
  Logger::Instanse().Log("[AssetPairHandler] AddAssetPair -> " + asset_pair, Logger::FileTag);

  trading::Currency base_currency = SeparateBaseCurrency(asset_pair);
  error::TradingError error_code = IncreasePairsCounter(base_currency);

  if(error::FAILED == error_code) {
    Logger::Instanse().Log("[AssetPairHandler::Error] Currency limit reached", Logger::FileTag);
  }

  drawer_.Flush();
  drawer_.Draw(pairs_count_);

  return error_code;
}

error::TradingError AssetPairHandler::RemovePair(const trading::common::asset_pair_t& asset_pair) {
  Logger::Instanse().Log("[AssetPairHandler] RemovePair -> " + asset_pair, Logger::FileTag);

  DecreasePairsCounter(SeparateBaseCurrency(asset_pair));
  drawer_.Flush();
  drawer_.Draw(pairs_count_);
  return error::SUCCESS;
}

trading::Currency AssetPairHandler::SeparateBaseCurrency(const trading::common::asset_pair_t& asset_pair) {
  Logger::Instanse().Log("[AssetPairHandler] SeparateBaseCurrency -> " + asset_pair, Logger::FileTag);

  for(auto& item : pairs_count_) {
    std::string string_currency = MultipurposeConverter::ConvertCurrencyToString(item.first);

    if(std::string::npos != asset_pair.find(string_currency) && 0 < asset_pair.find(string_currency)) {
      return item.first;
    }
  }

  return trading::PASS;  
}

error::TradingError AssetPairHandler::IncreasePairsCounter(const trading::common::Currency& currency) {
  Logger::Instanse().Log("[AssetPairHandler] IncreasePairsCounter -> " + std::to_string(currency), Logger::FileTag);

  error::TradingError error_code = error::SUCCESS;

  if(max_pairs_count_[currency] > pairs_count_[currency]) {
    pairs_count_[currency]++;
    Logger::Instanse().Log("[AssetPairHandler::Debug] IncreasePairsCounter -> " + MultipurposeConverter::ConvertCurrencyToString(currency)
    + ": " + std::to_string(pairs_count_[currency]), Logger::FileTag);
  } else {
    error_code = error::FAILED;
  }

  return error_code;
}

void AssetPairHandler::DecreasePairsCounter(const trading::common::Currency& currency) {
  Logger::Instanse().Log("[AssetPairHandler] DecreasePairsCounter -> " + std::to_string(currency), Logger::FileTag);

  if(pairs_count_[currency] > 0) {
    pairs_count_[currency]--;
    Logger::Instanse().Log("[AssetPairHandler::Debug] DecreasePairsCounter -> " + MultipurposeConverter::ConvertCurrencyToString(currency)
    + ": " + std::to_string(pairs_count_[currency]), Logger::FileTag);
  }
}
