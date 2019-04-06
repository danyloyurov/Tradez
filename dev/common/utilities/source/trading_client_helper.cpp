#include "trading_client_helper.hpp"

#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

std::size_t curl_calculate_data_size(void* ptr, std::size_t size, std::size_t nmemb, void* stream) {
    return size * nmemb;
}

std::vector<unsigned char> get_sha256_hash(const std::string& data) {
    std::vector<unsigned char> digest(SHA256_DIGEST_LENGTH);
    SHA256_CTX ctx;

    SHA256_Init(&ctx);
    SHA256_Update(&ctx, data.c_str(), data.length());
    SHA256_Final(digest.data(), &ctx);

    return digest;
}

std::vector<unsigned char> get_sha512_hmac(const std::vector<unsigned char>& data,  const std::vector<unsigned char>& key) {
    unsigned int length = EVP_MAX_MD_SIZE;
    std::vector<unsigned char> digest(length);

    HMAC_CTX *ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha512(), NULL);
    HMAC_Update(ctx, data.data(), data.size());
    HMAC_Final(ctx, digest.data(), &length);
    HMAC_CTX_free(ctx);

    return digest;
}

std::vector<unsigned char> base_64_decode(const std::string& data) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bmem = BIO_new_mem_buf(reinterpret_cast<const void*>(data.c_str()), data.length());
    std::vector<unsigned char> output(data.length());

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_push(b64, bmem);

    int decoded_size = BIO_read(bmem, output.data(), output.size());
    BIO_free_all(bmem);

    if (0 > decoded_size) {
        output.clear();
    }

    return output;
}

std::string base_64_encode(const std::vector<unsigned char>& data) {
    BIO* base64 = BIO_new(BIO_f_base64());
    BIO* base_mem = BIO_new(BIO_s_mem());
    BUF_MEM* buffer_memory = NULL;

    BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);
    base64 = BIO_push(base64, base_mem);
    BIO_write(base64, data.data(), data.size());
    BIO_flush(base64);
    BIO_get_mem_ptr(base64, &buffer_memory);

    std::string output(buffer_memory->data, buffer_memory->length);
    BIO_free_all(base64);

    return output;
}