#include "bittrex.hpp"
#include "multipurpose_converter.hpp"
#include <regex>
#include <ctime>
#include <list>


BittrexPlatform::BittrexPlatform(const std::string& public_key, const std::string& private_key)
    : btx_client_(public_key, private_key) {
}

error::TradingError BittrexPlatform::PlaceOrder(trading::Order& order) {
    try {
        if(BUY == order.type){
            auto response = btx_client_.get_market().buy_limit(order.asset_pair, static_cast<float>(order.volume), static_cast<float>(order.price));

            if (true == response.empty()) return error::FAILED;

            std::string ID;
            int lindex = response.find("uuid") + 8;
            ID = response.substr(lindex, ID.size() - 1);
            int rindex = ID.find("\"");
            ID = ID.substr(0, rindex);

            order.trading_patform_ID_ = ID;
            open_orders_for_closed.push_back(ID);
        }
        else if(SELL == order.type){
            auto response = btx_client_.get_market().sell_limit(order.asset_pair, static_cast<float>(order.volume), static_cast<float>(order.price));

            if (true == response.empty()) return error::FAILED;

            std::string ID;
            int lindex = response.find("uuid") + 8;
            ID = response.substr(lindex, ID.size() - 1);
            int rindex = ID.find("\"");
            ID = ID.substr(0, rindex);

            order.trading_patform_ID_ = ID;
            open_orders_for_closed.push_back(ID);

        }
        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}
 
error::TradingError BittrexPlatform::RemoveOrder(const trading::id_t& order_ID) {
    try {
        auto response = btx_client_.get_market().cancel(order_ID);

        if (response.empty()) return error::FAILED;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) {
    try {
        auto response = btx_client_.get_public().get_markets()

        for(auto i : response){
            asset_pairs.push_back(i.market_name);
        }

        if(response.empty()) return error::FAILED;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetCurrecyMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) {
    try {
        auto response = btx_client_.get_public().get_market_history(pair)

        time_t curtime;
        time(&curtime);

        trading::price_t low = 1000000;
        trading::price_t high = 0;
        for(auto& item : response) {

            if((curtime - MultipurposeConverter::ConvertBittrexTimeToSeconds(response.time_stamp)) < time_period) {

                if(item.price < 0.0001) {
                    currency_margin = -100;
                    break;
                }

                if(low > item.price) {
                    low = item.price;
                }

                if(high < item.price) {
                    high = item.price;
                }

            }

        }

        currency_margin = ( (high - low) / (low / 100) );

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetCurrecyPrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) {
    try {
        auto response = btx_client_.get_public().get_market_history(pair);

        time_t curtime;
        time(&curtime);

        trading::price_t low = 1000000;
        trading::price_t high = 0;
        for(auto& item : response) {

            if((curtime - MultipurposeConverter::ConvertBittrexTimeToSeconds(item.time_stamp)) < time_period) {

                if(low > item.price) {
                    low = item.price;
                }

                if(high < item.price) {
                    high = item.price;
                }

            }
        }

        price = (high + low) / 2;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresponseset& presponseset) {
    try {
        auto response = btx_client_.get_public().get_market_history(pair)

        time_t curtime;
        time(&curtime);

        int before_comma_max_count = 0;
        int after_comma_max_count = 0;
        std::string price;

        for(auto& item : response) {
            price = MultipurposeConverter::ConvertFloatToString(item.price);

            int comma_index = price.find(".");

            if(std::string::npos == comma_index || price == "") continue;

            if(comma_index > before_comma_max_count) {
                before_comma_max_count = comma_index;
            }

            if((price.length() - 1) - comma_index > after_comma_max_count) {
                after_comma_max_count = (price.length() - 1) - comma_index;
            }
        }

        presponseset.left_side_symbols_count_ = before_comma_max_count;
        presponseset.right_side_symbols_count_ = after_comma_max_count;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::price_t& base_volume, trading::volume_t& crypto_volume) {
    try {
        auto response = btx_client_.get_public().get_market_history(pair)

        if(true == response.empty()) return error::FAILED;

        trading::price_t price = response[response.size() - 1].price;

        double mult = 1 / price;
        crypto_volume = mult * base_volume;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetClosedOrders(std::vector<trading::id_t>& closed_orders) {
    try {
        for (auto i : open_orders_){
            auto response = btx_client_.get_account().get_order(i)
            if (true == response.closed) closed_orders.push_back(i);
        }

        return error::SUCCESS;
    } catch(...) {
         return error::FAILED;
    }
}
