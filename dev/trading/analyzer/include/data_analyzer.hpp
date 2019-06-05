#ifndef DATA_ANALYZER_HPP_
#define DATA_ANALYZER_HPP_

#include <memory>
#include <vector>
#include <ctime>
#include <algorithm>
#include <map>
#include <utility>

#include "error_interface.hpp"
#include "trading_platform.hpp"

class DataAnalyzer {
  public:
    std::vector<trading::common::price_t> Get24Prices(const std::vector<trading::analyzer::RawAsset>& raw_asset_dump);
    std::vector<trading::common::price_t> GetSectorPrices(const std::vector<trading::common::price_t>& prices, const int& sector);
    trading::common::price_t GetMedian(std::vector<trading::common::price_t> prices);
    trading::common::price_t GetFirstQuantile(std::vector<trading::common::price_t> prices);
    trading::analyzer::Trend IsTrendGrowing(const std::vector<trading::common::price_t>& prices);

  private:
    const std::map<int, std::pair<int, int> > sectors_ = {{1, {18, 24}},
                                                    {2, {12, 18}},
                                                    {3, {6, 12}},
                                                    {4, {0,6}}};
};

#endif // DATA_ANALYZER_HPP_
