#ifndef BOT_TRADER_HPP_
#define BOT_TRADER_HPP_

#include "trading_platform_observer.hpp"
#include "orders_handler.hpp"
#include "asset_pair_handler.hpp"

class Trader : public ITradingPlatformObserver {
  public:
    Trader(std::shared_ptr<ITradingPlatform> trading_platform);
    ~Trader();

    void PollFailedOrders();

    virtual void NotifyOrderClosed(const trading::id_t& order_ID);
    virtual void NotifyPairFound(const trading::asset_pair_t& asset_pair);
  private:
    std::list<trading::id_t> failed_sell_orders_;
    AssetPairHandler asset_pair_handler_;
    OrdersHandler orders_handler_;
};

#endif // BOT_TRADER_HPP_
