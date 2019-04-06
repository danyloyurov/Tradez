#ifndef TRADING_CLIENT_HELPER_HPP_
#define TRADING_CLIENT_HELPER_HPP_

#include <vector>
#include <string>

std::size_t curl_calculate_data_size(void* ptr, std::size_t size, std::size_t nmemb, void* stream);
std::vector<unsigned char> get_sha256_hash(const std::string& data);
std::vector<unsigned char> get_sha512_hmac(const std::vector<unsigned char>& data,  const std::vector<unsigned char>& key);
std::vector<unsigned char> base_64_decode(const std::string& data);
std::string base_64_encode(const std::vector<unsigned char>& data);

#endif // TRADING_CLIENT_HELPER_HPP_
