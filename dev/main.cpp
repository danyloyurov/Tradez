#include "trading_platform_observer.hpp"
#include "kraken.hpp"
#include "trader.hpp"

int main() {
    const std::string kPublicKey = "<your key>";
    const std::string kPrivateKey = "<your key>";

    std::shared_ptr<ITradingPlatform> trading_platform = std::make_shared<KrakenPlatform>(kPublicKey, kPrivateKey);

    TradingPlatformObserver trading_platform_observer(trading_platform);
    auto trader = std::make_shared<Trader>(trading_platform);
    CommonSystemDrawer system_drawer;

    trading_platform_observer.SubsctibeObserver(trader);

    while(true) {
        system_drawer.Flush();
        system_drawer.Draw(time(NULL));
        trading_platform_observer.DispatchEvents();
        trader->PollFailedOrders();
        trading_platform_observer.PeekEvents();
    }

    return 0;
}
