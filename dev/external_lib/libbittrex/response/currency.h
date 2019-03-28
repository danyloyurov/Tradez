#ifndef BITTREX_CPP_RES_CURRENCY_H
#define BITTREX_CPP_RES_CURRENCY_H

#include "../lib/json.hpp"
#include "../lib/wrappers.h"

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct Currency {
    explicit Currency(json j_currency) {
        currency = j_currency["Currency"];
        currency_long = j_currency["CurrencyLong"];
        min_confirmation = j_currency["MinConfirmation"];
        txfee = j_currency["TxFee"];
        is_active = j_currency["IsActive"];
        coin_type = j_currency["CoinType"];
        base_address = j_currency["BaseAddress"];
    };

    std::string currency;
    std::string currency_long;
    int min_confirmation;
    double txfee;
    bool is_active;
    std::string coin_type;
    std::string base_address;

};
} //Namespace Response
} //Namespace bittrex

#endif //BITTREX_CPP_CURRENCY_H
