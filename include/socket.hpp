#pragma once

#include "sockaddr.hpp"
#include <utility> // std::exchange
#include <stdexcept>

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib") 
#else
	#include <unistd.h>
	#include <errno.h>
#endif

namespace ZHTTP {

class Socket {
public:
#ifdef _WIN32
    using native_handle_t = SOCKET ;
    static constexpr native_handle_t invalid_handle = INVALID_SOCKET ;
#else
    using native_handle_t = int32_t ;
    static constexpr native_handle_t invalid_handle = -1 ;
#endif

private:
    native_handle_t handle_ = invalid_handle ;

	explicit Socket(native_handle_t h) : handle_(h) {}

public:
    Socket() {
        handle_ = ::socket(AF_INET, SOCK_STREAM, 0) ;

		if (invalid())
			throw std::runtime_error("Socket - Error to create socket.") ;
    }

    ~Socket() {
        close() ;
    }

    Socket(const Socket&) = delete ;
    Socket& operator=(const Socket&) = delete ;

    Socket(Socket&& other) noexcept
	 : handle_(std::exchange(other.handle_, invalid_handle)) {}

    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            close() ;
            handle_ = std::exchange(other.handle_, invalid_handle) ;
        }
        return *this ;
    }

    void close() {
        if (handle_ != invalid_handle) {
#ifdef _WIN32
			::closesocket(handle_) ;
#else 
			::close(handle_) ;
#endif
            handle_ = invalid_handle ;
        }
    }

	void bind(const SockAddr& addr) {
        auto native_addr = addr.native() ;
        if (::bind(handle_, reinterpret_cast<sockaddr*>(&native_addr), sizeof(native_addr)) != 0) 
            throw std::runtime_error("Socket::bind - Bind failed.") ;
    }

	void listen(int backlog = SOMAXCONN) {
        if (::listen(handle_, backlog) != 0) 
            throw std::runtime_error("Socket::listen - Listen failed.") ;
    }

	void connect(const SockAddr& addr) {
        auto native_addr = addr.native();
        if (::connect(handle_, reinterpret_cast<sockaddr*>(&native_addr), sizeof(native_addr)) != 0) 
            throw std::runtime_error("Socket::connect - Connect failed.") ;
    }

	[[nodiscard]] Socket accept(SockAddr* out_client_addr = nullptr) {
        SockAddr::native_t client_native{} ;
        socklen_t len = sizeof(client_native) ;
        native_handle_t client_fd = ::accept(handle_, reinterpret_cast<sockaddr*>(&client_native), &len) ;

        if (client_fd == invalid_handle) 
            throw std::runtime_error("Accept failed") ;

        if (out_client_addr) 
            *out_client_addr = SockAddr(client_native) ;

        return Socket(client_fd) ;
    }
    
	bool invalid() const noexcept { return handle_ == invalid_handle ; }

    native_handle_t native_handle() const noexcept { return handle_ ; }
} ;

} // namespace ZHTTP