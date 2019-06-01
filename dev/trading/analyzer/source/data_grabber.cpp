#include "data_grabber.hpp"

DataGrabber::DataGrabber(std::shared_ptr<ITradingPlatform> trading_platform){
  std::shared_ptr<ITradingPlatform> trading_platform_ = trading_platform;
}

error::TradingError GetAssetData(const trading::common::asset_pair_t& asset_pair, std::vector<trading::analyzer::Sector>& asset_data){
    std::vector<trading::analyzer::RawAsset> raw_asset_dump_;
    trading_platform_.GetAssetRawDump(asset_pair, raw_asset_dump_);
    std::vector<trading::common::price_t> prices = data_analyzer_.Get24Prices(raw_asset_dump_);
    std::vector<trading::common::price_t> first_sector = data_analyzer_.GetSectorPrices(prices, 1);
    std::vector<trading::common::price_t> second_sector = data_analyzer_.GetSectorPrices(prices, 2);
    std::vector<trading::common::price_t> third_sector = data_analyzer_.GetSectorPrices(prices, 3);
    std::vector<trading::common::price_t> fourth_sector = data_analyzer_.GetSectorPrices(prices, 4);


    asset_data[0].median_ = data_analyzer_.GetMedian(first_sector);
    asset_data[0].first_quantile_ = data_analyzer_.GetFirstQuantile(first_sector);
    asset_data[0].trend_ = data_analyzer_.IsTrendGrowing(first_sector);

    asset_data[1].median_ = data_analyzer_.GetMedian(second_sector);
    asset_data[1].first_quantile_ = data_analyzer_.GetFirstQuantile(second_sector);
    asset_data[1].trend_ = data_analyzer_.IsTrendGrowing(second_sector);
    
    asset_data[2].median_ = data_analyzer_.GetMedian(third_sector);
    asset_data[2].first_quantile_ = data_analyzer_.GetFirstQuantile(third_sector);
    asset_data[2].trend_ = data_analyzer_.IsTrendGrowing(third_sector);

    asset_data[3].median_ = data_analyzer_.GetMedian(fourth_sector);
    asset_data[3].first_quantile_ = data_analyzer_.GetFirstQuantile(fourth_sector);
    asset_data[3].trend_ = data_analyzer_.IsTrendGrowing(fourth_sector);
}