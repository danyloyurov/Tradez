#ifndef TRADING_INTERFACE_HPP_
#define TRADING_INTERFACE_HPP_

#include <string>

namespace trading {

static const double kPairMarginPassRate = 3.0;
static const int kDefaultTimePeriod = 7200; /*seconds*/
static const int kPrivateRequestSleep = 60; /*seconds*/
static const int kPublicRequestSleep = 30; /*seconds*/

static const int kMaxUSDPairsCount = 0;
static const int kMaxEURPairsCount = 0;
static const int kMaxXBTPairsCount = 0;

static const int kUSDBaseBuyVolume = 0;
static const int kEURBaseBuyVolume = 0;
static const int kXBTBaseBuyVolume = 0;

static const int kPriceMultiplier = 1.5;

using id_t = std::string;
using asset_pair_t = std::string;
using price_t = double;
using volume_t = double;

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

struct PricePresset {
    PricePresset() {}
    PricePresset(const int& left_side_symbols_count,
                 const int& right_side_symbols_count)
    : left_side_symbols_count_(left_side_symbols_count),
      right_side_symbols_count_(right_side_symbols_count) {}

    int left_side_symbols_count_;
    int right_side_symbols_count_;
};

struct Order {
    Order(const asset_pair_t& asset_pair,
          const volume_t& volume,
          const price_t& price,
          const OrderType& type,
          const PricePresset& price_presset)
    : asset_pair_(asset_pair),
      volume_(volume),
      price_(price),
      type_(type),
      price_presset_(price_presset) {}

    Order(const id_t& trading_patform_ID)
    : trading_patform_ID_(trading_patform_ID) {}

    const bool operator>(const Order& order) const {
        return this->trading_patform_ID_ > order.trading_patform_ID_;
    }

    const bool operator<(const Order& order) const {
        return this->trading_patform_ID_ < order.trading_patform_ID_;
    }

    const bool operator>=(const Order& order) const {
        return this->trading_patform_ID_ >= order.trading_patform_ID_;
    }

    const bool operator<=(const Order& order) const {
        return this->trading_patform_ID_ <= order.trading_patform_ID_;
    }

    const bool operator==(const Order& order) const {
        return this->trading_patform_ID_ == order.trading_patform_ID_;
    }

    id_t trading_patform_ID_;
    asset_pair_t asset_pair_;
    volume_t volume_;
    price_t price_;
    OrderType type_;
    PricePresset price_presset_;
};

auto price_calculator = [](const price_t& price, const OrderType& type)
{
    return BUY == type ? price - price / 100 * kPriceMultiplier : price + price / 100 * (kPriceMultiplier * 2);
};

} // trading

#endif // TRADING_INTERFACE_HPP_
