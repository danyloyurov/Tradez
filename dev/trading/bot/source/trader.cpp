#include "trader.hpp"

#include <iostream>

Trader::Trader(std::shared_ptr<ITradingPlatform> trading_platform)
    : trading_platform_(trading_platform),
      asset_pair_handler_(trading_platform) {
    std::cout << "Trader()\n";
}

Trader::~Trader() {
    std::cout << "~Trader()\n";
}

void Trader::NotifyOrderClosed(const trading::id_t& order_ID) {
    std::cout << "NotifyOrderClosed -> " << order_ID << std::endl;
}

void Trader::NotifyPairFound(const trading::pair_t& asset_pair) {
    std::cout << "NotifyPairFound -> " << asset_pair << std::endl;

    error::TradingError error_code = error::SUCCESS;

    error_code = asset_pair_handler_.AddAssetPair(asset_pair);

    if(error::FAILED == error_code) {
        return;
    }
}
