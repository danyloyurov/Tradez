#include "trading_platform_observer.hpp"
#include "kraken.hpp"
#include "trader.hpp"

int main() {
  const std::string kPublicKey = "<your key>";
  const std::string kPrivateKey = "<your key>";

  auto kraken_platform = std::make_shared<KrakenPlatform>(kPublicKey, kPrivateKey);
  auto kraken_trader = std::make_shared<Trader>(kraken_platform);
  TradingPlatformObserver trading_platform_observer(kraken_platform);
  CommonSystemDrawer system_drawer;

  trading_platform_observer.SubsctibeObserver(kraken_trader);

  while(true) {
    system_drawer.Flush();
    system_drawer.Draw(time(NULL));

    trading_platform_observer.DispatchEvents();
    kraken_trader->PollFailedOrders();
    trading_platform_observer.PeekEvents();
  }

  return 0;
}
