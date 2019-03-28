#ifndef BITTREX_CPP_RES_MARKET_H
#define BITTREX_CPP_RES_MARKET_H

#include "../lib/json.hpp"
#include "../lib/wrappers.h"

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct Market {
    explicit Market(json j_market) {
        market_currency = j_market["MarketCurrency"];
        base_currency = j_market["BaseCurrency"];
        market_currency_long = j_market["MarketCurrencyLong"];
        base_currency_long = j_market["BaseCurrencyLong"];
        min_trade_size = j_market["MinTradeSize"];
        market_name = j_market["MarketName"];
        is_active = j_market["IsActive"];
        created = j_market["Created"];
    };

    std::string market_currency;
    std::string base_currency;
    std::string market_currency_long;
    std::string base_currency_long;
    double min_trade_size;
    std::string market_name;
    bool is_active;
    std::string created;
};
} //Namespace Response
} //Namespace bittrex

#endif //BITTREX_CPP_MARKET_H
