#pragma once

/**
 * @file core/sockaddr.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief 
 * @version 1.0.0
 * @date 2025-12-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ipv4.hpp"

#ifdef max
	#undef max
#endif
#ifdef min
	#undef min
#endif
#ifdef ERROR
	#undef ERROR
#endif

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

namespace frqs {

class SockAddr {
public :
	using native_t = sockaddr_in ;

private :
	IPv4 address_ {} ; // data yang disimpan adalah nbo, otomatis cvt hbo ke nbo
	uint16_t port_ {} ; // data yang disimpan wajib nbo

	constexpr auto invert(std::unsigned_integral auto value) const noexcept -> decltype(value) {
		return IS_LITTLE_ENDIAN ? std::byteswap(value) : value ;
	}

	constexpr void append_uint16(std::string& dst, uint16_t port) const noexcept {
		if (port == 0) {
			dst.push_back('0') ;
			return ;
		}

		char buffer[5] ; 
		int count = 0 ;

		uint16_t temp = port ; 
		while (temp > 0) {
			buffer[count++] = (temp % 10) + '0' ;
			temp /= 10 ;
		}

		for (int i = count - 1; i >= 0; --i) 
			dst.push_back(buffer[i]) ;
	}

public :
	constexpr SockAddr() noexcept = default ;
	constexpr SockAddr(const SockAddr&) noexcept = default ;
	constexpr SockAddr(SockAddr&&) noexcept = default ;
	constexpr SockAddr& operator=(const SockAddr&) noexcept = default ;
	constexpr SockAddr& operator=(SockAddr&&) noexcept = default ;
	constexpr std::strong_ordering operator<=>(const SockAddr&) const noexcept = default ;
	constexpr ~SockAddr() noexcept = default ;

	constexpr SockAddr(const IPv4& address, uint16_t port) noexcept
	 : address_(address), port_(invert(port)) {}
	
	constexpr SockAddr(const native_t& native) noexcept
	 : address_(invert(native.sin_addr.s_addr))
	 , port_(native.sin_port) {}

	constexpr const IPv4& getAddress() const noexcept { return address_ ; }
	constexpr IPv4& getAddress() noexcept { return address_ ; }
	constexpr uint16_t getPort() const noexcept { return invert(port_) ; }
	constexpr void setPort(uint16_t port) noexcept { port_ = invert(port) ; }

	constexpr std::string toString() const noexcept {
		std::string r ;
		r.reserve(21) ;
		r += address_.toString() ;
		r += ':' ;
		append_uint16(r, getPort()) ;
		return r ;
	}

	constexpr native_t native() const noexcept {
		native_t r {} ;
		r.sin_family = AF_INET ;
		r.sin_port = port_ ;
		r.sin_addr.s_addr = address_.Nbo_toUint32() ;
		return r ;
	}
} ;

} // namespace frqs
