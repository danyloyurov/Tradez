#ifndef DRAWERS_HPP_
#define DRAWERS_HPP_

#include "drawing_tool.hpp"
#include "trading_interface.hpp"

#include <map>
#include <vector>

class AssetPairsDrawer : public IDrawingTool<std::map<trading::Currency, int>> {
    public:
        virtual void Draw(const std::map<trading::Currency, int>& pairs_data);
        virtual void Flush();
    private:
        const int kStartX = 0;
        const int kStartY = 1;
        const int kEndX = 100;
        const int kEndY = 15;
};

class OrdersDrawer : public IDrawingTool<std::vector<trading::Order>> {
    public:
        virtual void Draw(const std::vector<trading::Order>& orders);
        virtual void Flush();
    private:
        const int kStartX = 0;
        const int kStartY = 15;
        const int kEndX = 300;
        const int kEndY = 30;
};

#endif //DRAWERS_HPP_
