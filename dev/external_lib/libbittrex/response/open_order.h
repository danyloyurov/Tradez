#ifndef BITTREX_CPP_RES_OPEN_ORDER_H
#define BITTREX_CPP_RES_OPEN_ORDER_H

#include "../lib/json.hpp"
#include "../lib/wrappers.h"

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct OpenOrder {
    explicit OpenOrder(json j_order) {
        uuid = j_order["Uuid"];
        order_uuid = j_order["OrderUuid"];
        exchange = j_order["Exchange"];
        order_type = j_order["OrderType"];
        quantity = j_order["Quantity"];
        quantity_remaining = j_order["QuantityRemaining"];
        limit = j_order["Limit"];
        commission_paid = j_order["CommissionPaid"];
        price = j_order["Price"];
        price_per_unit = j_order["PricePerUnit"];
        opened = j_order["Opened"];
        closed = j_order["Closed"];
        cancel_initiated = j_order["CancelInitiated"];
        immediate_or_cancel = j_order["ImmediateOrCancel"];
        is_conditional = j_order["IsConditional"];
        condition = j_order["Condition"];
        condition_target = j_order["ConditionTarget"];
    };

    std::string uuid;
    std::string order_uuid;
    std::string exchange;
    std::string order_type;
    double quantity;
    double quantity_remaining;
    double limit;
    double commission_paid;
    double price;
    std::string price_per_unit;
    std::string opened;
    std::string closed;
    bool cancel_initiated;
    bool immediate_or_cancel;
    bool is_conditional;
    std::string condition;
    std::string condition_target;

};
}
}
#endif //BITTREX_CPP_OPEN_ORDER_H
