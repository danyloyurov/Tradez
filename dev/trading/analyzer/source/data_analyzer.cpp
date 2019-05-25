#include "data_analyzer.hpp"

std::vector<trading::common::price_t> get_12hour_prices_(std::vector<trading::analyzer::RawAsset> raw_asset_dump){
    time_t current_time_ = time(NULL);
    const time_t twelve_hours_ = 43200;
    const time_t half_an_hour_ = 1800;
    const time_t boundary_for_step = 100;
    time_t bottom_line_ = current_time_ - twelve_hours_;
    time_t first_mark_ = current_time_ - half_an_hour_;

    std::vector<trading::common::price_t> prices_;

    while(first_mark_>=bottom_line_){
        for(auto asset : raw_asset_dump){
            if(asset.time_ >= first_mark_ - boundary_for_step && asset.time_<= first_mark_ + boundary_for_step){
                prices_.push_back(asset.price_);
                first_mark_ -= half_an_hour_;
            }
        }
    }
    
return prices_;
}

trading::common::price_t get_median_(std::vector<trading::common::price_t> prices_){
    size_t size = prices_.size();

    sort(prices_.begin(), prices_.end());

    return (prices_[size / 2 - 1] + prices_[size / 2]) / 2;
}

trading::common::price_t get_first_quartile_(std::vector<trading::common::price_t> prices_){
    sort(prices_.begin(), prices_.end());
    trading::common::price_t sum_of_prices_ = 0.0;
    for (auto n : prices_)
        sum_of_prices_ += n;

    return (sum_of_prices_ + 1) / 4;
}

bool is_trend_growing(std::vector<trading::common::price_t> prices_){
    int more_counter_ = 0;
    int less_counter_ = 0;

    for (auto price : prices_){
        if(price > prices_[11])
            more_counter_ += 1;

        else if(price < prices_[11])
            less_counter_ += 1;
    }

    if(more_counter_ > less_counter_)
        return true;
    else
        return false;
}
