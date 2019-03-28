#ifndef BITTREX_CPP_RES_ORDER_BOOK_H
#define BITTREX_CPP_RES_ORDER_BOOK_H

#include "../lib/json.hpp"
#include "../lib/utils.h"
#include "../lib/wrappers.h"

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct OrderBookEntry {
    explicit OrderBookEntry(json j) {
        quantity = j["Quantity"];
        rate = j["Rate"];
    };

    Double quantity;
    Double rate;

};

struct OrderBook {
    OrderBook(json o_book, const std::string &type);

    std::vector<OrderBookEntry> buy;
    std::vector<OrderBookEntry> sell;

};
} //Namespace Response
} //Namespace bittrex

#endif //BITTREX_CPP_ORDER_BOOK_H
