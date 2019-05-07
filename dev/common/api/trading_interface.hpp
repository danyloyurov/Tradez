#ifndef TRADING_INTERFACE_HPP_
#define TRADING_INTERFACE_HPP_

#include <string>
#include <vector>
#include <ctime>

namespace trading {
namespace common {

static const int kDefaultTimePeriod = 7200; /*seconds*/
static const int kPrivateRequestSleep = 60; /*seconds*/
static const int kPublicRequestSleep = 30; /*seconds*/

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

} //common

namespace analyzer {

static const int kRiskTimePeriod = 86400; /*seconds*/
static const double kPairMarginPassRate = 3.0;
static const double kFailingTrend = 5.0;

enum Trend {
  GROWING,
  FAILING
};

struct Sector {
  common::price_t first_quantile_;
  common::price_t median_;
  Trend trend_;
};

} // analyzer

namespace bot {

static const int kOrderExpirationTimePeriod = 14400; /*seconds*/

static const int kMaxUSDPairsCount = 0;
static const int kMaxEURPairsCount = 0;
static const int kMaxXBTPairsCount = 0;

static const double kUSDBaseBuyVolume = 0;
static const double kEURBaseBuyVolume = 0;
static const double kXBTBaseBuyVolume = 0;

static const double kTradingPlatformFee = 0.3; /*percents*/
static const double kPriceMultiplier = 1.5;

struct Order {
  Order(const common::asset_pair_t& asset_pair,
        const common::volume_t& volume,
        const common::price_t& price,
        const time_t& time_placed,
        const common::OrderType& type,
        const common::PricePresset& price_presset)
  : asset_pair_(asset_pair),
    volume_(volume),
    price_(price),
    time_placed_(time_placed),
    type_(type),
    price_presset_(price_presset) {}

  Order(const common::id_t& trading_patform_ID)
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

  common::id_t trading_patform_ID_;
  common::asset_pair_t asset_pair_;
  common::volume_t volume_;
  common::price_t price_;
  time_t time_placed_;
  common::OrderType type_;
  common::PricePresset price_presset_;
};

auto price_calculator = [](const common::price_t& price, const common::OrderType& type) {
  return common::BUY == type ? price - price / 100 * kPriceMultiplier : price + price / 100 * (kPriceMultiplier * 2);
};

auto fee_calculator = [](const common::volume_t& volume, const common::volume_t& fee_percent) {
  return volume - volume / 100 * fee_percent;
};

} // bot
} // trading

#endif // TRADING_INTERFACE_HPP_
