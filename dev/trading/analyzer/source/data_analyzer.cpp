#include "data_analyzer.hpp"

std::vector<trading::common::price_t> DataAnalyzer::Get24Prices(const std::vector<trading::analyzer::RawAsset>& raw_asset_dump){
  time_t current_time = time(NULL);
  const time_t twelve_hours_and_a_half = 45000;
  const time_t half_an_hour = 1800;
  const time_t boundary_for_step = 100;
  time_t bottom_line = current_time - twelve_hours_and_a_half;
  time_t first_mark = current_time;

  std::vector<trading::common::price_t> prices;

  while (first_mark>=bottom_line) {
    for (auto asset : raw_asset_dump) {
      if (asset.time_ >= first_mark - boundary_for_step && asset.time_<= first_mark + boundary_for_step) {
        prices.push_back(asset.price_);
        first_mark -= half_an_hour;
      }
    }
  }
  return prices;
}

std::vector<trading::common::price_t> DataAnalyzer::GetSectorPrices(const std::vector<trading::common::price_t>& prices,const int& sector_num){
  std::vector<trading::common::price_t> sector;
  auto range =  sectors_[sector_num];

  sector.assign(prices.begin() + std::get<0>(range), prices.begin() + std::get<1>(range)+1);

  return sector;
}

trading::common::price_t DataAnalyzer::GetMedian(std::vector<trading::common::price_t> prices){
  size_t size = prices.size();

  std::sort(prices.begin(), prices.end());
  if (size % 2 == 0) {
    return (prices[size / 2 - 1] + prices[size / 2]) / 2;
  } else { 
    return prices[size / 2];
  }
}

trading::common::price_t DataAnalyzer::GetFirstQuantile(std::vector<trading::common::price_t> prices){
  std::sort(prices.begin(), prices.end());
  trading::common::price_t sum_of_prices = 0.0;
  for (auto price : prices) {
    sum_of_prices += price;
  }

  return (sum_of_prices + 1) / 4;
}

trading::analyzer::Trend DataAnalyzer::IsTrendGrowing(const std::vector<trading::common::price_t>& prices){
  std::vector<double> percents;
  float sum_of_percents = 0;

  for (auto i = prices.size() - 1; i > 0; --i) {
      percents.push_back((prices[i - 1] - prices[i]) / prices[i] * 100);
  }

  for (auto percent : percents) {
      sum_of_percents += percent;
  }

  if (static_cast<int>(sum_of_percents) > 0) {
    return trading::analyzer::GROWING;
  } else {
    return trading::analyzer::FALLING;
  }
}
