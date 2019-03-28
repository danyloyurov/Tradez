#include "bittrex.hpp"
#include "multipurpose_converter.hpp"
#include <regex>
#include <ctime>
#include <list>


BittrexPlatform::BittrexPlatform(const std::string &public_key, const std::string &private_key)
    : btx_client_(public_key, private_key) {
}

error::TradingError BittrexPlatform::PlaceOrder(trading::Order& order) {
    try {
        if(trading::BUY == order.type_){
            auto response = btx_client_.market_api().buy_limit(order.asset_pair_, static_cast<float>(order.volume_), static_cast<float>(order.price_));

            if (true == response.empty()) return error::FAILED;

            std::string ID;
            int lindex = response.find("uuid") + 8;
            ID = response.substr(lindex, ID.size() - 1);
            int rindex = ID.find("\"");
            ID = ID.substr(0, rindex);

            order.trading_patform_ID_ = ID;
            open_orders_.push_back(ID);
        }
        else if(trading::SELL == order.type_){
            auto response = btx_client_.market_api().sell_limit(order.asset_pair_, static_cast<float>(order.volume_), static_cast<float>(order.price_));

            if (true == response.empty()) return error::FAILED;

            std::string ID;
            int lindex = response.find("uuid") + 8;
            ID = response.substr(lindex, ID.size() - 1);
            int rindex = ID.find("\"");
            ID = ID.substr(0, rindex);

            order.trading_patform_ID_ = ID;
            open_orders_.push_back(ID);

        }
        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}
 
error::TradingError BittrexPlatform::RemoveOrder(const trading::id_t& order_ID) {
    try {
        auto response = btx_client_.market_api().cancel(order_ID);

        if (response.empty()) return error::FAILED;

        return error::SUCCESS;
    } catch(...) {
        return error::FAILED;
    }
}

error::TradingError BittrexPlatform::GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) {
    try {
        auto response = btx_client_.public_api().get_markets();

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
        auto response = btx_client_.public_api().get_market_history(pair);

        time_t curtime;
        time(&curtime);

        trading::price_t low = 1000000;
        trading::price_t high = 0;
        for(auto& item : response) {

            if((curtime - MultipurposeConverter::ConvertBittrexTimeToSeconds(item.time_stamp)) < time_period) {

                if(item.price < 0.0001) {
                    currency_margin = -100;
                    break;
                }

                if(low > static_cast<double>(item.price)) {
                    low = static_cast<double>(item.price);
                }

                if(high < static_cast<double>(item.price)) {
                    high = static_cast<double>(item.price);
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
        auto response = btx_client_.public_api().get_market_history(pair);

        time_t curtime;
        time(&curtime);

        trading::price_t low = 1000000;
        trading::price_t high = 0;
        for(auto& item : response) {

            if((curtime - MultipurposeConverter::ConvertBittrexTimeToSeconds(item.time_stamp) < time_period)) {

                if(low > static_cast<double>(item.price)) {
                    low = static_cast<double>(item.price);
                }

                if(high < static_cast<double>(item.price)) {
                    high = static_cast<double>(item.price);
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
        auto response = btx_client_.public_api().get_market_history(pair);

        time_t curtime;
        time(&curtime);

        int before_comma_max_count = 0;
        int after_comma_max_count = 0;
        std::string price;

        for(auto& item : response) {
            price = MultipurposeConverter::ConvertFloatToString(static_cast<float>(item.price));

            int comma_index = price.find(".");

            if(std::string::npos == static_cast<unsigned int>(comma_index) || price == "") continue;

            if(comma_index > before_comma_max_count) {
                before_comma_max_count = comma_index;
            }

            if((price.length() - 1) - comma_index > static_cast<unsigned int>(after_comma_max_count)) {
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
        auto response = btx_client_.public_api().get_market_history(pair);

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
            auto response = btx_client_.account_api().get_order(i);
            if (true == response.closed) closed_orders.push_back(i);
        }

        return error::SUCCESS;
    } catch(...) {
         return error::FAILED;
    }
}
