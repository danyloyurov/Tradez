#include <iostream>
#include <thread>
#include <chrono>

#include "trading_platform_observer.hpp"
#include "kraken.hpp"
#include "trader.hpp"

int main() {
    const std::string kPublicKey = "<your key>";
    const std::string kPrivateKey = "<your key>";

    std::shared_ptr<ITradingPlatform> trading_platform = std::make_shared<KrakenPlatform>(kPublicKey, kPrivateKey);

    TradingPlatformSubject trading_platform_observer(trading_platform);
    Trader trader(trading_platform);

    trading_platform_observer.SubsctibeObserver(std::make_shared<Trader>(trader));

    while(true) {
        trading_platform_observer.DispatchEvents();

        trading_platform_observer.PeekEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
