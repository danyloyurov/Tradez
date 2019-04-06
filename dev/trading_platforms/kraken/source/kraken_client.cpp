#include "kraken_client.hpp"
#include "trading_client_helper.hpp"
#include "logger.hpp"

static std::string input_to_string(const std::map<std::string, std::string>& input) {
    std::string output;

    for(auto& item : input) {
        if(*input.begin() != item) output += "&";
        output += item.first + "=" = item.second;
    }

    return output;
}

KrakenClient::KrakenClient(const std::string& public_key, const std::string& private_key)
    : public_key_(public_key),
      private_key_(private_key),
      curl_(curl_easy_init()) {
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl_, CURLOPT_USERAGENT, "Kraken C++ API Client");
    curl_easy_setopt(curl_, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl_calculate_data_size);

    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);

    if (code != CURLE_OK) {
        Logger::Instanse().Log("[KrakenClient::Error] curl_global_init error code: " + std::to_string(code), Logger::FileTag);    
        throw std::runtime_error("curl_global_init::error");
   }
}

KrakenClient::~KrakenClient() {
    curl_global_cleanup();
}

error::TradingError KrakenClient::PublicRequest(const KrakenRequest& request) {
    error::TradingError error_code = error::SUCCESS;
    std::string request_path = "/" + api_version_ + "/public/" + request.type;
    std::string request_url = api_url_ + request_path;
    std::string post_data = input_to_string(request.input);

    curl_easy_setopt(curl_, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, NULL);
    std::string response;
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, static_cast<void*>(&response));
 
    CURLcode result = curl_easy_perform(curl_);

    if (result != CURLE_OK) {
       error_code = error::FAILED;
    }

    return error_code;
}

error::TradingError KrakenClient::PrivateRequest(const KrakenRequest& request) {
    error::TradingError error_code = error::SUCCESS;

    return error_code; 
}
