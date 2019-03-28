#ifndef BITTREX_CPP_RES_ORDER_HISTORY_ENTRY_H
#define BITTREX_CPP_RES_ORDER_HISTORY_ENTRY_H

#include "../lib/json.hpp"
#include "../lib/wrappers.h"

using json=nlohmann::json;
using namespace bittrex::lib;

namespace bittrex {
namespace response {

struct OrderHistoryEntry {
    explicit OrderHistoryEntry(json j_order_ent) {
        order_uuid = j_order_ent["OrderUuid"];
        exchange = j_order_ent["Exchange"];
        time_stamp = j_order_ent["TimeStamp"];
        order_type = j_order_ent["OrderType"];
        limit = j_order_ent["Limit"];
        quantity = j_order_ent["Quantity"];
        quantity_remaining = j_order_ent["QuantityRemaining"];
        commission = j_order_ent["Commission"];
        price = j_order_ent["Price"];
        price_per_unit = j_order_ent["PricePerUnit"];
        is_conditional = j_order_ent["IsConditional"];
        condition = j_order_ent["Condition"];
        condition_target = j_order_ent["ConditionTarget"];
        immediate_or_cancel = j_order_ent["ImmediateOrCancel"];

    }

    std::string order_uuid;
    std::string exchange;
    std::string time_stamp;
    std::string order_type;
    double limit;
    double quantity;
    double quantity_remaining;
    double commission;
    double price;
    double price_per_unit;
    bool is_conditional;
    std::string condition;
    std::string condition_target;
    bool immediate_or_cancel;

};
} //Namespace Response
} //Namespace bittrex
#endif //BITTREX_CPP_ORDER_HISTORY_ENTRY_H
