#ifndef TRADING_INTERFACE_HPP_
#define TRADING_INTERFACE_HPP_

#include <string>

namespace trading {

static const double kPairMarginPassRate = 3.0;
static const int kDefaultTimePeriod = 7200; /*seconds*/

static const int kMaxUSDPairsCount = 5;
static const int kMaxEURPairsCount = 5;
static const int kMaxXBTPairsCount = 5;

static const int kUSDBaseBuyVolume = 25;
static const int kEURBaseBuyVolume = 20;
static const int kXBTBaseBuyVolume = 0.001;

static const int kPriceMultiplier = 1.25;

typedef std::string id_t;
typedef std::string asset_pair_t;
typedef double price_t;
typedef double volume_t;

enum OrderStatus {
    NONE,
    FIRSTHANG,
    SECONDHANG,
    THIRDHAND,
    STOPLOSS
};

enum OrderType {
    BUY,
    SELL
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

struct Order {
    Order(const asset_pair_t& asset_pair,
          const volume_t& volume,
          const price_t& price,
          const time_t& current_time,
          const OrderType& type)
    : asset_pair_(asset_pair),
      volume_(volume),
      price_(price),
      time_placed_(current_time),
      type_(type) {}

    Order(const asset_pair_t& asset_pair)
    : asset_pair_(asset_pair) {}

    const bool operator>(const Order& order) const {
        return this->asset_pair_ > order.asset_pair_;
    }

    const bool operator<(const Order& order) const {
        return this->asset_pair_ < order.asset_pair_;
    }

    const bool operator>=(const Order& order) const {
        return this->asset_pair_ >= order.asset_pair_;
    }

    const bool operator<=(const Order& order) const {
        return this->asset_pair_ <= order.asset_pair_;
    }

    const bool operator==(const Order& order) const {
        return this->asset_pair_ == order.asset_pair_;
    }

    id_t trading_patform_ID_;
    asset_pair_t asset_pair_;
    volume_t volume_;
    price_t price_;
    time_t time_placed_;
    OrderType type_;
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

auto price_calculator = [](const price_t& price, const OrderType& type)
{
    return BUY == type ? price - price / 100 * kPriceMultiplier : price + price / 100 * kPriceMultiplier;
};

} // trading

#endif // TRADING_INTERFACE_HPP_
