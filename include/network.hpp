#pragma once

#include "socket.hpp"

namespace ZHTTP {

class Network {
public:
    Network() {
#ifdef _WIN32
        WSADATA wsaData ;
        int result = ::WSAStartup(MAKEWORD(2, 2), &wsaData) ;
        if (result != 0) 
            throw std::runtime_error("Network - Failed to initialization.") ;
#endif
    }

    ~Network() {
#ifdef _WIN32
        ::WSACleanup() ;
#endif
    }

    Network(const Network&) = delete ;
    Network& operator=(const Network&) = delete ;
    Network(Network&&) = delete ;
    Network& operator=(Network&&) = delete ;
} ;

} // namespace ZHTTP