#include "bittrex.hpp"
#include "multipurpose_converter.hpp"
#include <regex>
#include <ctime>

BittrexPlatform::BittrexPlatform(const std::string& public_key, const std::string& private_key)
    : btx_client_(public_key, private_key) {
}

time_t string_to_time(string& time_to_parse) {
    std::string year, month_day, hours_minutes_seconds;
    std::smatch match_year, match_month_day, match_hours_minutes_seconds;

    std::regex year_ ("[0-9]{4}");
    std::regex month_day_ ("[0-9]{2}[-]{1}[0-9]{2}");
    std::regex hours_minutes_seconds_ ("[0-9]{2}[:]{1}[0-9]{2}[:]{1}[0-9]{2}");

    while(std::regex_search (time_to_parse, match_year, year_)) {
        for(auto& a : match_year) year = a;
        time_to_parse = match_year.suffix().str();
    }
    while(std::regex_search (time_to_parse, match_mnth_day, month_day_)) {
        for(auto& a : match_month_day) mnth_day = a;
        time_to_parse = match_month_day.suffix().str();
    }
    while(std::regex_search (time_to_parse, match_hours_minutes_seconds, hours_minutes_seconds)) {
        for(auto& a : match_hours_minutes_seconds) hours_minutes_seconds = a;
        time_to_parse = match_hours_minutes_seconds.suffix().str();
    }

    time_t timer = time(NULL);
    struct tm *timeinfo = localtime(&timer);

    timeinfo->tm_year = stoi(year) - 1900;
    timeinfo->tm_mon = stoi(month_day.substr(0,2))-1;
    timeinfo->tm_mday = stoi(month_day.substr(3,2));
    timeinfo->tm_hour = stoi(hours_minutes_seconds.substr(0,2));
    timeinfo->tm_min = stoi(hours_minutes_seconds.substr(3,2));
    timeinfo->tm_sec = stoi(hours_minutes_seconds.substr(6,2));

    timer = mktime(timeinfo);
    return timer;
}



error::TradingError BittrexPlatform::PlaceOrder(trading::Order& order) {
    try {
        if(BUY == order.type){
            auto res = btx_client_.get_market().buy_limit(order.asset_pair, static_cast<float>(order.volume), static_cast<float>(order.price));

            if (true == res.empty()) return error::FAILED;

            std::string ID;
            int lindex = res.find("uuid") + 8;
            ID = res.substr(lindex, ID.size() - 1);
            int rindex = ID.find("\"");
            ID = ID.substr(0, rindex);

            order.trading_patform_ID_ = ID;
        }
        else if(SELL == order.type){
            auto res = btx_client_.get_market().sell_limit(order.asset_pair, static_cast<float>(order.volume), static_cast<float>(order.price));

            if (true == res.empty()) return error::FAILED;

            std::string ID;
            int lindex = res.find("uuid") + 8;
            ID = res.substr(lindex, ID.size() - 1);
            int rindex = ID.find("\"");
            ID = ID.substr(0, rindex);

            order.trading_patform_ID_ = ID;
        }
        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}
 
error::TradingError BittrexPlatform::RemoveOrder(const trading::id_t& order_ID) {
    try {
        auto res = btx_client_.get_market().cancel(order_ID);

        if (res.empty()) return error::FAILED;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) {
    try {
        auto res = btx_client_.get_public().get_markets()

        for(auto i : res){
            asset_pairs.push_back(i.market_name);
        }

        if(res.empty()) return error::FAILED;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetCurrecyMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) {
    try {
        auto res = btx_client_.get_public().get_market_history(pair)

        time_t curtime;
        time(&curtime);

        trading::price_t low = 1000000;
        trading::price_t high = 0;
        for(auto& item : res) {

            if((curtime - string_to_time(res.time_stamp)) < time_period) {

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
        auto res = btx_client_.get_public().get_market_history(pair);

        time_t curtime;
        time(&curtime);

        trading::price_t low = 1000000;
        trading::price_t high = 0;
        for(auto& item : res) {

            if((curtime - string_to_time(item.time_stamp)) < time_period) {

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

error::TradingError BittrexPlatform::GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresset& presset) {
    try {
        auto res = btx_client_.get_public().get_market_history(pair)

        time_t curtime;
        time(&curtime);

        int before_comma_max_count = 0;
        int after_comma_max_count = 0;
        std::string price;

        for(auto& item : res) {
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

        presset.left_side_symbols_count_ = before_comma_max_count;
        presset.right_side_symbols_count_ = after_comma_max_count;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::price_t& base_volume, trading::volume_t& crypto_volume) {
    try {
        auto res = btx_client_.get_public().get_market_history(pair)

        if(true == res.empty()) return error::FAILED;

        trading::price_t price = res[res.size() - 1].price;

        double mult = 1 / price;
        crypto_volume = mult * base_volume;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetClosedOrders(std::vector<trading::id_t>& closed_orders) {
    try {
        Kraken::KInput input_closed_orders;
        input_closed_orders["ofs"] = "0";
        json_string response = libjson::to_json_string(kraken_client_.private_method("ClosedOrders", input_closed_orders)); 
        JSONNode root = libjson::parse(response);

        if (false == root.at("error").empty()) return error::FAILED;
        if (true == root.at("result").empty()) return error::FAILED;

        std::smatch match;
        std::regex ebat ("[0-9A-Z]{6}[-]{1}[0-9A-Z]{5}[-]{1}[0-9A-Z]{6}");

        while(std::regex_search (response, match, ebat)) {
            for(auto& closed_orded : match) {
                closed_orders.push_back(closed_orded);
            }

            response = match.suffix().str();
        }

        return error::SUCCESS;
    } catch(...) {
         return error::FAILED;
    }
}
