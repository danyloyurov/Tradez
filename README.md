# Tradez
Cryptocurrency trading bot

Trading strategy - **speculation**

Trading pairs: TBD/**USD**, TBD/**EUR**, TBD/**XBT**

Supporting trading platforms: **Kraken**

Incoming trading platforms: **Bittrex, Bitstamp, Bitfinex**

## How to use:

**Configure bot in file `Tradez/dev/common/api/trading_iterface.hpp` by changing next variables values:**

`kMaxUSDPairsCount` `kMaxEURPairsCount` `kMaxXBTPairsCount`

_Max pairs in use means (**TBD** currency balance) **/** k**TBD**BaseBuyVolume_

`kUSDBaseBuyVolume` `kEURBaseBuyVolume` `kXBTBaseBuyVolume`

_Base buy volume means amount of money available to be spent on one buy order_

**Configure access to trading platform in file `Tradez/dev/main.cpp` by changing next variables values:**

`kPublicKey` `kPrivateKey`

_This keys you can get from your trading platform account_

## API authors:

### Kraken:

**License:** The MIT License (MIT)

**Copyright:** (c) 2013 - Marco E. <marco.esposito@gmail.com>

**Link:** https://github.com/voidloop/krakenapi

## License:

MIT
