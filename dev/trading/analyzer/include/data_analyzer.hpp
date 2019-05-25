#ifndef DATA_ANALYZER_HPP_
#define DATA_ANALYZER_HPP_

#include <memory>
#include <vector>
#include <ctime>
#include <algorithm>

#include "error_interface.hpp"
#include "trading_platform.hpp"

class DataAnalyzer {
  public:
    std::vector<trading::common::price_t> get_12hour_prices_(std::vector<trading::analyzer::RawAsset> raw_asset_dump);
    trading::common::price_t get_median_(std::vector<trading::common::price_t> prices_);
    trading::common::price_t get_first_quartile_(std::vector<trading::common::price_t> prices_);
    bool is_trend_growing(std::vector<trading::common::price_t> prices_);
};

#endif // DATA_ANALYZER_HPP_














