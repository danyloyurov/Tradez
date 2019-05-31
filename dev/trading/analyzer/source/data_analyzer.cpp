#include "data_analyzer.hpp"

std::vector<trading::common::price_t> DataAnalyzer::Get24Prices(std::vector<trading::analyzer::RawAsset> raw_asset_dump){
    time_t current_time = time(NULL);
    const time_t twelve_hours_and_a_half = 45000;
    const time_t half_an_hour = 1800;
    const time_t boundary_for_step = 100;
    time_t bottom_line = current_time - twelve_hours_and_a_half;
    time_t first_mark = current_time;

    std::vector<trading::common::price_t> prices;

    while(first_mark>=bottom_line){
        for(auto asset : raw_asset_dump){
            if(asset.time_ >= first_mark - boundary_for_step && asset.time_<= first_mark + boundary_for_step){
                prices.push_back(asset.price_);
                first_mark -= half_an_hour;
            }
        }
    }
return prices;
}

std::vector<trading::common::price_t> DataAnalyzer::GetSectorPrices(const std::vector<trading::common::price_t>& prices, int sector_num){
    std::vector<trading::common::price_t> sector;

    switch(sector_num){
        case 4:{
            for(int i=0; i < prices.size(); i++){{
                while(i <= 6)
                    sector.push_back(prices[i]);
            }
        }
        break;
    }
        case 3:{
            for(int i=0; i < prices.size(); i++){{
                while(i >= 6 && i <= 12)
                    sector.push_back(prices[i]);
            }
        }
        break;
    }
        case 2:{
            for(int i=0; i < prices.size(); i++){{
                while(i >= 12 && i <= 18)
                    sector.push_back(prices[i]);
            }
        }
        break;
    }
        case 1:{
            for(int i=0; i < prices.size(); i++){{
                while(i >= 18 && i <= 24)
                    sector.push_back(prices[i]);
            }
        }
        break;
    }        
    default:
        break;
}
    return sector;
}
trading::common::price_t DataAnalyzer::GetMedian(const std::vector<trading::common::price_t>& prices){
    size_t size = prices.size();
    const uint32_t divider_with_value_two = 2;

    sort(prices.begin(), prices.end());
    if (size % divider_with_value_two == 0)
    {
      return (prices[size / divider_with_value_two - 1] + prices[size / divider_with_value_two]) / divider_with_value_two;
    }
    else 
    {
      return prices[size / divider_with_value_two];
    }
}

trading::common::price_t DataAnalyzer::GetFirstQuantile(const std::vector<trading::common::price_t>& prices){
    sort(prices.begin(), prices.end());
    trading::common::price_t sum_of_prices = 0.0;
    for (auto n : prices)
        sum_of_prices += n;

    return (sum_of_prices + 1) / 4;
}

trading::analyzer::Trend DataAnalyzer::IsTrendGrowing(const std::vector<trading::common::price_t>& prices){
    int more_counter = 0;
    int less_counter = 0;

    for (auto price : prices){
        if(price > prices[24])
            more_counter += 1;

        else if(price < prices[24])
            less_counter += 1;
    }

    if(more_counter > less_counter)
        return trading::analyzer::GROWING;
    else
        return trading::analyzer::FALLING;
}
