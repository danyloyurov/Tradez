#include "kraken.hpp"

KrakenPlatform::KrakenPlatform(const std::string& public_key, const std::string& private_key)
    : kraken_client(public_key, private_key) {
    Kraken::initialize();
}

KrakenPlatform::~KrakenPlatform() {
    Kraken::terminate();
}

error::TradingError KrakenPlatform::PlaceOrder(trading::Order& order, const trading::PricePresset& presset) {

}

error::TradingError KrakenPlatform::RemoveOrder(const trading::id_t& order_ID) {

}

error::TradingError KrakenPlatform::GetAssetPairs(std::vector<trading::pair_t>& asset_pairs) {

}

error::TradingError KrakenPlatform::GetCurrecyMargin(const trading::pair_t& pair, const int& time_period, trading::price_t& currency_margin) {

}

error::TradingError KrakenPlatform::GetCurrecyPrice(const trading::pair_t& pair, const int& time_period, trading::price_t& price) {

}

error::TradingError KrakenPlatform::GetPairPriceFormat(const trading::pair_t& pair, trading::PricePresset& presset) {

}

error::TradingError KrakenPlatform::GetVolumeToBuy(const trading::pair_t& pair, const trading::price_t& fiat_volume, trading::volume_t& crypto_volume) {

}

error::TradingError KrakenPlatform::GetAccountBalance(const trading::currecy_t& currency, trading::volume_t& value) {

}

error::TradingError KrakenPlatform::GetClosedOrders(const std::vector<trading::id_t>& orders, std::vector<trading::id_t>& closed_orders) {

}

error::TradingError KrakenPlatform::GetOrderVolume(const trading::id_t& id, trading::volume_t& remaining_volume) {

}
