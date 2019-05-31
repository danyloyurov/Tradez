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
    std::vector<trading::common::price_t> Get24Prices(std::vector<trading::analyzer::RawAsset> raw_asset_dump);
    std::vector<trading::common::price_t> GetSectorPrices(std::vector<trading::common::price_t> prices_, int sector);
    trading::common::price_t GetMedian(std::vector<trading::common::price_t> prices_);
    trading::common::price_t GetFirstQuantile(std::vector<trading::common::price_t> prices_);
    trading::analyzer::Trend IsTrendGrowing(std::vector<trading::common::price_t> prices_);
    };

#endif // DATA_ANALYZER_HPP_
