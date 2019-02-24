#ifndef TRADING_INTERFACE_HPP_
#define TRADING_INTERFACE_HPP_

#include <string>

namespace trading {

typedef double price_t;
typedef double volume_t;
typedef std::string id_t;
typedef std::string pair_t;
typedef std::string currecy_t;
typedef std::string type_t;

struct Order {
    Order(const int& id,
          const volume_t& volume,
          const price_t& price,
          const time_t& current_time,
          const pair_t& pair,
          const type_t& type)
    : local_ID_(id),
      volume_(volume),
      price_(price),
      time_placed_(current_time),
      pair_(pair),
      type_(type) {}

    int local_ID_;
    id_t trading_patform_ID_;
    volume_t volume_;
    price_t price_;
    time_t time_placed_;
    type_t type_;
    pair_t pair_;
};

struct PricePresset {
    PricePresset() {}
    PricePresset(const int& left_side_symbols_count,
                 const int& right_side_symbols_count)
    : left_side_symbols_count_(left_side_symbols_count),
      right_side_symbols_count_(right_side_symbols_count) {}

    int left_side_symbols_count_;
    int right_side_symbols_count_;
};

enum OrderStatus {
    NONE,
    FIRSTHANG,
    SECONDHANG,
    THIRDHAND,
    STOPLOSS
};

enum Currency {
    PASS,
    USD,
    EUR,
    XBT
};

enum Operation {
    INSERT,
    CHECKUP
};

} // trading

#endif // TRADING_INTERFACE_HPP_
