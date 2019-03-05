#ifndef ORDERS_HANDLER_HPP_
#define ORDERS_HANDLER_HPP_

#include "trading_platform.hpp"
#include "asset_pair_handler.hpp"
#include "container_helper.hpp"

class OrdersHandler {
    public:
        OrdersHandler(std::shared_ptr<ITradingPlatform> trading_platform);
        ~OrdersHandler();

        void PlaceBuyOrder(const trading::asset_pair_t& asset_pair, const trading::Currency& base_currency);
        void PlaceSellOrder(const trading::id_t& order_ID);
    private:
        void RemoveOrder(const trading::id_t& order_ID);

        using OrdersSearcher = Searcher<trading::Order>;
        using OrdersSorter = Sorter<trading::Order>;
        using OrdersVector = std::vector<trading::Order>;

        std::vector<trading::Order> open_orders_;
        std::map<trading::Currency, trading::volume_t> base_currency_volumes_;
        std::shared_ptr<ITradingPlatform> trading_platform_;
};

#endif // ORDERS_HANDLER_HPP_
