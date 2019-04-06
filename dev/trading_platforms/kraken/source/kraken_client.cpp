#include "kraken_client.hpp"
#include "trading_client_helper.hpp"
#include "logger.hpp"

#include <vector>

static std::string input_to_string(const std::map<std::string, std::string>& input) {
    std::string output;

    for(auto& item : input) {
        if(*input.begin() != item) output += "&";
        output += item.first + "=" = item.second;
    }

    return output;
}

static std::string get_nonce() {
   std::string nonce;
   timeval tp;
   gettimeofday(&tp, NULL);

   nonce = std::to_string(tp.tv_sec).substr(0, 10) + std::to_string(tp.tv_usec).substr(0, 6);
   return nonce;
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

KrakenResponse KrakenClient::PublicRequest(const KrakenRequest& request) {
    KrakenResponse kraken_response;
    std::string request_url = api_url_ + "/" + api_version_ + "/public/" + request.type;
    std::string post_data = input_to_string(request.input);

    curl_easy_setopt(curl_, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, NULL);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, static_cast<void*>(&kraken_response.response));

    CURLcode result = curl_easy_perform(curl_);

    if (CURLE_OK != result) {
       kraken_response.error_code = error::FAILED;
    }

    return kraken_response;
}

KrakenResponse KrakenClient::PrivateRequest(const KrakenRequest& request) {
    KrakenResponse kraken_response;
    std::string request_url = api_url_ + "/" + api_version_ + "/private/" + request.type;
    std::string nonce = get_nonce();
    std::string post_data = "nonce=" + nonce + "&" + input_to_string(request.input);

    curl_easy_setopt(curl_, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, post_data.c_str());

    curl_slist* chunk = NULL;
    std::string signature = CreateSignature(request_url, nonce, post_data);

    if("" != signature) {
        std::string key_header =  "API-Key: "  + public_key_;
        std::string sign_header = "API-Sign: " + signature;

        chunk = curl_slist_append(chunk, key_header.c_str());
        chunk = curl_slist_append(chunk, sign_header.c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, static_cast<void*>(&kraken_response.response));

        CURLcode result = curl_easy_perform(curl_);

        if (CURLE_OK != result) {
            kraken_response.error_code = error::FAILED;
        }

    } else {
        kraken_response.error_code = error::FAILED;
    }

    curl_slist_free_all(chunk);
    return kraken_response;
}

std::string KrakenClient::CreateSignature(const std::string& request_path, const std::string& nonce, const std::string& post_data) const
{
    std::vector<unsigned char> data(request_path.begin(), request_path.end());
    std::vector<unsigned char> sha265_hash = get_sha256_hash(nonce + post_data);
    std::vector<unsigned char> decoded_private_key = base_64_decode(private_key_);

    if(true == decoded_private_key.empty()) return "";

    data.insert(data.end(), sha265_hash.begin(), sha265_hash.end());
    std::vector<unsigned char> sha512_hmac = get_sha512_hmac(data, decoded_private_key);

    return base_64_encode(sha512_hmac);
}
