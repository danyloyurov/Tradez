#ifndef BITTREX_CPP_RES_MARKET_HISTORY_H
#define BITTREX_CPP_RES_MARKET_HISTORY_H

#include "../lib/json.hpp"
#include "../lib/wrappers.h"

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct Trade {
    explicit Trade(json j_market_his) {
        id = j_market_his["Id"];
        time_stamp = j_market_his["TimeStamp"];
        quantity = j_market_his["Quantity"];
        price = j_market_his["Price"];
        total = j_market_his["Total"];
        fill_type = j_market_his["FillType"];
        order_type = j_market_his["OrderType"];
    };

    int id;
    std::string time_stamp;
    double quantity;
    double price;
    double total;
    std::string fill_type;
    std::string order_type;
};
} //Namespace Response
} //Namespace bittrex
#endif //BITTREX_CPP_MARKET_HISTORY_H
