#include "trading_client_helper.hpp"

std::size_t curl_calculate_data_size(void* ptr, std::size_t size, std::size_t nmemb, void* stream) {
    return size * nmemb;
}

