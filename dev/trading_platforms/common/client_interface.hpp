#ifndef CLIENT_INTERFACE_HPP_
#define CLIENT_INTERFACE_HPP_

#include "error_interface.hpp"

/*

* T is request structure

* N is response structure

*/
template<class T, class N>
class ClientInterface {
    public:
        virtual ~ClientInterface() {}
        virtual N PublicRequest(const T& request) = 0;
        virtual N PrivateRequest(const T& request) = 0;
};

#endif // CLIENT_INTERFACE_HPP_
