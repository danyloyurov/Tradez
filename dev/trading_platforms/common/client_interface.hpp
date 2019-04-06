#ifndef CLIENT_INTERFACE_HPP_
#define CLIENT_INTERFACE_HPP_

#include "error_interface.hpp"

template<class T>
class ClientInterface {
    public:
        virtual ~ClientInterface() {}
        virtual error::TradingError PublicRequest(const T& request) = 0;
        virtual error::TradingError PrivateRequest(const T& request) = 0;
};

#endif // CLIENT_INTERFACE_HPP_
