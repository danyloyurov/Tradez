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
    std::vector<trading::common::price_t> GetSectorPrices(const std::vector<trading::common::price_t>& prices, int sector);
    trading::common::price_t GetMedian(const std::vector<trading::common::price_t>& prices);
    trading::common::price_t GetFirstQuantile(const std::vector<trading::common::price_t>& prices);
    trading::analyzer::Trend IsTrendGrowing(const std::vector<trading::common::price_t>& prices);
    };

#endif // DATA_ANALYZER_HPP_
