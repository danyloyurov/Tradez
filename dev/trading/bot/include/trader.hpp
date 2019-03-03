#ifndef BOT_TRADER_HPP_
#define BOT_TRADER_HPP_

#include "trading_platform_observer.hpp"
#include "trading_platform.hpp"
#include "asset_pair_handler.hpp"
#include "container_helper.hpp"

class Trader : public ITradingPlatformObserver {
    public:
        Trader(std::shared_ptr<ITradingPlatform> trading_platform);
        ~Trader();

        virtual void NotifyOrderClosed(const trading::id_t& order_ID);
        virtual void NotifyPairFound(const trading::asset_pair_t& asset_pair);

    private:
        using OrdersSearcher = Searcher<trading::Order>;
        using OrdersSorter = Sorter<trading::Order>;
        using OrdersVector = std::vector<trading::Order>;

        void PlaceBuyOrder(const trading::asset_pair_t& asset_pair);
        void PlaceSellOrder(const trading::id_t& order_ID);
        void RemoveOrder(const trading::id_t& order_ID);

        std::vector<trading::Order> open_orders_;
        std::map<trading::Currency, trading::volume_t> base_currency_volumes_;
        AssetPairHandler asset_pair_handler_;
        std::shared_ptr<ITradingPlatform> trading_platform_;
};

#endif // BOT_TRADER_HPP_
