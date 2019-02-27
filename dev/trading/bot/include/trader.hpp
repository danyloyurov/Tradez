#ifndef BOT_TRADER_HPP_
#define BOT_TRADER_HPP_

#include "trading_platform_observer.hpp"
#include "trading_platform.hpp"
#include "asset_pair_handler.hpp"

class Trader : public ITradingPlatformObserver {
    public:
        Trader(std::shared_ptr<ITradingPlatform> trading_platform);
        ~Trader();

        virtual void NotifyOrderClosed(const trading::id_t& order_ID);
        virtual void NotifyPairFound(const trading::pair_t& asset_pair);
    private:
        AssetPairHandler asset_pair_handler_;
        std::shared_ptr<ITradingPlatform> trading_platform_;
};

#endif // BOT_TRADER_HPP_
