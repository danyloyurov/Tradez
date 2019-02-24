#ifndef TRADING_PLATFORM_OBSERVER_HPP_
#define TRADING_PLATFORM_OBSERVER_HPP_

#include "trading_interface.hpp"

#include <memory>
#include <list>

class ITradingPlatformObserver {
    public:
        virtual void NotifyOrderClosed(const trading::id_t& order_ID) = 0;
        virtual void NotifyPairFound(const trading::pair_t& asset_pair) = 0;
};

class TradingPlatformSubject {
    public:
        void DispatchEvents();

        void SubsctibeObserver(std::unique_ptr<ITradingPlatformObserver> subscriber);
    private:
        static struct ClosedOrder {} ClosedOrderTag;
        static struct PairFound {} PairFoundTag;

        void BroadcastEvent(const trading::id_t& order_ID, ClosedOrder);
        void BroadcastEvent(const trading::pair_t& asset_pair, PairFound);

        std::list<std::unique_ptr<ITradingPlatformObserver>> subscribers_;
        std::list<const trading::id_t> closed_orders_;
        std::list<const trading::pair_t> available_asset_pairs_;
};

#endif // TRADING_PLATFORM_OBSERVER_HPP_
