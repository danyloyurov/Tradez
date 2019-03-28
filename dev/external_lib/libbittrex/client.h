#ifndef BITTREX_CPP_CLIENT_H
#define BITTREX_CPP_CLIENT_H

#include <iostream>
#include <memory>
#include "api/public.h"
#include "api/market.h"
#include "api/account.h"
#include "api_call.h"

namespace bittrex {

class Client {
public:
    explicit Client(const std::string &key, const std::string &secret) :
            m_api_call(std::make_shared<bittrex::ApiCall>(key, secret)) {}

    /**
     * Used for public information available without an API key
     */
    api::Public public_api();

    /**
     * Used for programatic trading of crypto currencies
     */
    api::Market market_api();

    /**
     * Used for managing your account
     */
    api::Account account_api();


private:
    std::shared_ptr<bittrex::ApiCall> m_api_call;
};
} //Namespace Bittrex
#endif //BITTREX_CPP_CLIENT_H
