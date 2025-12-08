#pragma once

/**
 * @file core/network.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief 
 * @version 1.0.0
 * @date 2025-12-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "socket.hpp"

namespace frqs {

class Network {
public : 
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

static Network NetworkInit {} ;

} // namespace frqs