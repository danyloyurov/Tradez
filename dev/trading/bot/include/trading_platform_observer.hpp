#ifndef TRADING_PLATFORM_OBSERVER_HPP_
#define TRADING_PLATFORM_OBSERVER_HPP_

#include "trading_interface.hpp"
#include "trading_event.hpp"

#include <memory>
#include <vector>
#include <list>

class ITradingPlatformObserver {
  public:
    virtual void NotifyOrderClosed(const trading::id_t& order_ID) = 0;
    virtual void NotifyPairFound(const trading::asset_pair_t& asset_pair) = 0;
};

class TradingPlatformObserver {
  public:
    TradingPlatformObserver(std::shared_ptr<ITradingEvent> trading_platform);
    void PeekEvents();
    void DispatchEvents();

    void SubsctibeObserver(std::shared_ptr<ITradingPlatformObserver> subscriber);
  private:
    static struct ClosedOrder {} ClosedOrderTag;
    static struct AssetPair {} AssetPairTag;

    error::TradingError PeekEvents(ClosedOrder);
    error::TradingError PeekEvents(AssetPair);
    void BroadcastEvent(const trading::id_t& order_ID, ClosedOrder);
    void BroadcastEvent(const trading::asset_pair_t& asset_pair, AssetPair);

    std::list<std::shared_ptr<ITradingPlatformObserver>> subscribers_;
    std::vector<trading::id_t> closed_orders_;
    std::vector<trading::id_t> cached_closed_orders_;
    std::vector<trading::asset_pair_t> high_margin_asset_pairs_;
    std::vector<trading::asset_pair_t> all_asset_pairs_;
    std::shared_ptr<ITradingEvent> trading_platform_;
};

#endif // TRADING_PLATFORM_OBSERVER_HPP_
