#include "data_grabber.hpp"

error::TradingError GetAssetData(const trading::common::asset_pair_t& asset_pair, std::vector<trading::analyzer::Sector>& asset_data){
    std::vector<trading::analyzer::RawAsset> raw_asset_dump_;
    trading_platform_.GetAssetRawDump(asset_pair, raw_asset_dump_);

    std::vector<trading::common::price_t> prices_ = data_analyzer_.get_12hour_prices_(raw_asset_dump_);
    asset_data[0].median_ = data_analyzer_.get_median_(prices_);
    asset_data[0].first_quantile_ = data_analyzer_.get_first_quartile_(prices_);
    asset_data[0].trend_ = data_analyzer_.is_trend_growing(prices_);


}