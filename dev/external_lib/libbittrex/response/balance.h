#ifndef BITTREX_CPP_RES_BALANCE_H
#define BITTREX_CPP_RES_BALANCE_H

#include "../lib/json.hpp"
#include "../lib/wrappers.h"
#include <string>

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct Balance {
    explicit Balance(json j_balance) {
        currency = j_balance["Currency"];
        balance = j_balance["Balance"];
        available = j_balance["Available"];
        pending = j_balance["Pending"];
        crypto_address = j_balance["CryptoAddress"];
        requested = j_balance["Requested"];
        uuid = j_balance["Uuid"];
    };

    std::string currency;
    double balance;
    double available;
    double pending;
    std::string crypto_address;
    double requested;
    std::string uuid;

};
}
}
#endif //BITTREX_CPP_BALANCE_H
