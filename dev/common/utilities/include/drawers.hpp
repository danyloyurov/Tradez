#ifndef DRAWERS_HPP_
#define DRAWERS_HPP_

#include "drawing_tool.hpp"
#include "trading_interface.hpp"

#include <map>
#include <vector>

class CommonSystemDrawer : public IDrawingTool<time_t> {
    public:
        virtual void Draw(const time_t& time);
        virtual void Flush();
    private:
        const int kStartX = 110;
        const int kStartY = 1;
        const int kEndX = 200;
        const int kEndY = 10;
};

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
        const int kEndX = 400;
        const int kEndY = 30;
};

#endif //DRAWERS_HPP_
