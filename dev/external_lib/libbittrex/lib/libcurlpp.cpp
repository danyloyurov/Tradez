#include "libcurlpp.h"
#include "exceptions.h"

using namespace bittrex::lib;
using namespace curl::options;


size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata) {
    ((std::string *) userdata)->append(contents, size * nmemb);
    return size * nmemb;
}


Curl::Curl() :
        m_curl(curl_easy_init(), [](CURL *ptr) {
            curl_easy_cleanup(ptr);
        }) {

    if (!m_curl)
        throw fail("Curl init failed!");
}


void Curl::setOpt(std::shared_ptr<curl::options::OptionBase> opt) {
    opt->m_curlHandle = m_curl;
    opt->setOpt();
}


void Curl::perform() {
    m_res = curl_easy_perform(m_curl.get());
    // Check for errors
    if (m_res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(m_res));

}


