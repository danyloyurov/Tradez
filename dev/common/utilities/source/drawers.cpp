#include "drawers.hpp"
#include "multipurpose_converter.hpp"
#include "esc_defines.h"

#include <iostream>
#include <stdio.h>

void CommonSystemDrawer::Draw(const time_t& time) {
    int x = kStartX;
    int y = kStartY;

    set_cursor(x, y++);
    std::cout << "System time " << std::endl;
    set_cursor(x, y++);
    std::cout << "_____________" << std::endl;
    set_cursor(x, y++);
    std::cout << std::to_string(time) << std::endl;
}

void CommonSystemDrawer::Flush() {
    for(int y = kStartY; y < kEndY; y++) {
        for(int x = kStartX; x < kEndX; x++) {
            set_cursor(x, y);
            std::cout << " ";
        }
    }
}

void AssetPairsDrawer::Draw(const std::map<trading::Currency, int>& pairs_data) {
    int x = kStartX;
    int y = kStartY;

    set_cursor(x, y++);
    std::cout << "Base currency   Count" << std::endl;
    set_cursor(x, y++);
    std::cout << "_____________________" << std::endl;

    for(auto& data : pairs_data) {
        set_cursor(x, y);
        std::cout << MultipurposeConverter::ConvertCurrencyToString(data.first) << std::endl;
        set_cursor(x + 17, y++);
        std::cout << data.second << std::endl;
    }
}

void AssetPairsDrawer::Flush() {
    for(int y = kStartY; y < kEndY; y++) {
        for(int x = kStartX; x < kEndX; x++) {
            set_cursor(x, y);
            std::cout << " ";
        }
    }
}

void OrdersDrawer::Draw(const std::vector<trading::Order>& orders) {
    int x = kStartX;
    int y = kStartY;

    set_cursor(x, y++);
    std::cout << "Pair      Price     Volume    Type      Time placed    ID" << std::endl;
    set_cursor(x, y++);
    std::cout << "_________________________________________________________" << std::endl;

    for(auto& order : orders) {
        set_cursor(x, y);
        std::cout << order.asset_pair_ << std::endl;
        set_cursor(x + 11, y);
        std::cout << order.price_ << std::endl;
        set_cursor(x + 21, y);
        std::cout << order.volume_ << std::endl;
        set_cursor(x + 31, y);
        std::cout << (trading::BUY == order.type_ ? "Buy" : "Sell") << std::endl;
        set_cursor(x + 41, y);
        std::cout << order.time_placed_ << std::endl;
        set_cursor(x + 55, y++);
        std::cout << order.trading_patform_ID_ << std::endl;
    }
}

void OrdersDrawer::Flush() {
    for(int y = kStartY; y < kEndY; y++) {
        for(int x = kStartX; x < kEndX; x++) {
            set_cursor(x, y);
            std::cout << " ";
        }
    }
}

