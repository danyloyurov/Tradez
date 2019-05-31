#include "data_analyzer.hpp"

std::vector<trading::common::price_t> DataAnalyzer::Get24Prices(std::vector<trading::analyzer::RawAsset> raw_asset_dump){
    time_t current_time_ = time(NULL);
    const time_t twelve_hours_and_a_half = 45000;
    const time_t half_an_hour_ = 1800;
    const time_t boundary_for_step = 100;
    time_t bottom_line_ = current_time_ - twelve_hours_and_a_half;
    time_t first_mark_ = current_time_;

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

std::vector<trading::common::price_t> DataAnalyzer::GetSectorPrices(std::vector<trading::common::price_t> prices_, int sector_num){
    std::vector<trading::common::price_t> sector;

    switch(sector_num){
        case 4:{
            for(int i=0; i < prices_.size(); i++){{
                while(i <= 6)
                    sector.push_back(prices_[i]);
            }
        }
        break;
    }
        case 3:{
            for(int i=0; i < prices_.size(); i++){{
                while(i >= 6 && i <= 12)
                    sector.push_back(prices_[i]);
            }
        }
        break;
    }
        case 2:{
            for(int i=0; i < prices_.size(); i++){{
                while(i >= 12 && i <= 18)
                    sector.push_back(prices_[i]);
            }
        }
        break;
    }
        case 1:{
            for(int i=0; i < prices_.size(); i++){{
                while(i >= 18 && i <= 24)
                    sector.push_back(prices_[i]);
            }
        }
        break;
    }        
    default:
        break;
}
    return sector;
}
trading::common::price_t DataAnalyzer::GetMedian(std::vector<trading::common::price_t> prices_){
    size_t size = prices_.size();
    const uint32_t divider_with_value_two = 2;

    sort(prices_.begin(), prices_.end());
    if (size % divider_with_value_two == 0)
    {
      return (prices_[size / divider_with_value_two - 1] + prices_[size / divider_with_value_two]) / divider_with_value_two;
    }
    else 
    {
      return prices_[size / divider_with_value_two];
    }
}

trading::common::price_t DataAnalyzer::GetFirstQuantile(std::vector<trading::common::price_t> prices_){
    sort(prices_.begin(), prices_.end());
    trading::common::price_t sum_of_prices_ = 0.0;
    for (auto n : prices_)
        sum_of_prices_ += n;

    return (sum_of_prices_ + 1) / 4;
}

trading::analyzer::Trend DataAnalyzer::IsTrendGrowing(std::vector<trading::common::price_t> prices_){
    int more_counter_ = 0;
    int less_counter_ = 0;

    for (auto price : prices_){
        if(price > prices_[24])
            more_counter_ += 1;

        else if(price < prices_[24])
            less_counter_ += 1;
    }

    if(more_counter_ > less_counter_)
        return trading::analyzer::GROWING;
    else
        return trading::analyzer::FALLING;
}
