#ifndef TRADING_PLATFORM_KRAKEN_CLIENT_HPP_
#define TRADING_PLATFORM_KRAKEN_CLIENT_HPP_

#include "client_interface.hpp"

#include <map>
#include <string>
#include <curl/curl.h>

struct KrakenRequest {
    std::string type;
    std::string user_signature;
    std::map<std::string, std::string> input;
};

class KrakenClient : public ClientInterface<KrakenRequest> {
    public:
        explicit KrakenClient(const std::string& public_key, const std::string& private_key);
        KrakenClient(const KrakenClient&) = delete;
        ~KrakenClient();

        virtual error::TradingError PublicRequest(const KrakenRequest& request) final;
        virtual error::TradingError PrivateRequest(const KrakenRequest& request) final;
    private:
        const std::string api_version_ = "0";
        const std::string api_url_ = "https://api.kraken.com";
        std::string public_key_;
        std::string private_key_;
        CURL* curl_;
};

#endif // TRADING_PLATFORM_KRAKEN_CLIENT_HPP_
