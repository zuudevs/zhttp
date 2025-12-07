#pragma once

#include <algorithm>
#include <array>
#include <compare>
#include <concepts>
#include <cstdint>
#include <string>
#include <string_view>
#include "endian.hpp"

namespace ZHTTP {

class IPv4 {
public :
	using element_type = uint8_t ;
	using pointer = uint8_t* ;
	using reference = uint8_t& ;
	using const_pointer = const uint8_t* ;
	using const_reference = const uint8_t& ;
	static constexpr uint8_t capacity = 4 ;

private :
	std::array<uint8_t, 4> address_ {} ; // wajib nbo

    constexpr void append_uint8(std::string& out, uint8_t value) const noexcept {
        if (value >= 100) {
            out += static_cast<char>('0' + (value / 100)) ;
            value %= 100 ;
            out += static_cast<char>('0' + (value / 10)) ;
            value %= 10 ;
        } else if (value >= 10) {
            out += static_cast<char>('0' + (value / 10)) ;
            value %= 10 ;
        }
        out += static_cast<char>('0' + value) ;
    }

	[[nodiscard]] constexpr inline auto invert(std::unsigned_integral auto value) const noexcept -> decltype(value) {
		return IS_LITTLE_ENDIAN ? std::byteswap(value) : value ;
	}

public :
	constexpr IPv4() noexcept = default ;
	constexpr IPv4(const IPv4&) noexcept = default ;
	constexpr IPv4(IPv4&&) noexcept = default ;
	constexpr IPv4& operator=(const IPv4&) noexcept = default ;
	constexpr IPv4& operator=(IPv4&&) noexcept = default ;
	constexpr std::strong_ordering operator<=>(const IPv4&) const noexcept = default ;
	constexpr ~IPv4() noexcept = default ;

	template <std::integral E, std::unsigned_integral auto N>
	constexpr IPv4(const E (&address)[N]) noexcept {
		auto count = std::min(static_cast<decltype(N)>(capacity), N) ;
		for(std::size_t i = 0; i < count; i++)
			address_[i] = static_cast<uint8_t>(std::clamp(address[i], E{0}, E{255})) ;
	}

	template <std::integral E>
	constexpr IPv4(const E* address, std::size_t length) noexcept {
		auto count = std::min(static_cast<std::size_t>(capacity), length) ;
		for(std::size_t i = 0; i < count; i++)
			address_[i] = static_cast<uint8_t>(std::clamp(address[i], E{0}, E{255})) ;
	}

	constexpr explicit IPv4(uint32_t address) noexcept {
		address_ = std::bit_cast<std::array<uint8_t, 4>>(invert(address)) ;
	}

	constexpr IPv4(const std::string_view& s) noexcept {
        std::array<uint8_t, 4> result {} ;
        std::size_t octet_idx = 0 ;
        uint32_t current_val = 0 ;
        bool parsing_num = false ;

        for (char c : s) {
            if (c >= '0' && c <= '9') {
                current_val = (current_val * 10) + (c - '0') ;
                if (current_val > 255) current_val = 255 ; 
                parsing_num = true ;
            } else if (c == '.') {
                if (octet_idx < 4) {
                    result[octet_idx++] = static_cast<uint8_t>(current_val) ;
                    current_val = 0 ;
                    parsing_num = false ;
                }
            }
        }
        if (parsing_num && octet_idx < 4) 
            result[octet_idx] = static_cast<uint8_t>(current_val) ;
		address_ = std::bit_cast<std::array<uint8_t, 4>>(invert(std::bit_cast<uint32_t>(result))) ;
    }

	template <std::integral E, std::unsigned_integral auto N>
	constexpr IPv4& operator=(const E (&address)[N]) noexcept {
		using T = decltype(N) ;
		auto count = std::min(static_cast<T>(capacity), N) ;
		for(std::size_t i = 0; i < count; i++)
			address_[i] = static_cast<uint8_t>(std::clamp(address[i], E{0}, E{255})) ;
		address_ = std::bit_cast<std::array<uint8_t, 4>>(invert(std::bit_cast<uint32_t>(address_))) ;
		return *this ;
	}

	constexpr IPv4& operator=(uint32_t address) noexcept {
		address_ = std::bit_cast<std::array<uint8_t, 4>>(invert(address)) ;
		return *this ;
	}

	constexpr operator uint32_t() const noexcept { return toUint32() ; }
	constexpr uint32_t toUint32() const noexcept { return std::bit_cast<uint32_t>(address_) ; }

	constexpr std::string toString() const noexcept {
        std::string res ;
        res.reserve(15) ;
        append_uint8(res, address_[0]) ;
        res += '.' ;
        append_uint8(res, address_[1]) ;
        res += '.' ;
        append_uint8(res, address_[2]) ;
        res += '.' ;
        append_uint8(res, address_[3]) ;

        return res ;
    }

	constexpr pointer data() noexcept { return address_.data() ; }
	constexpr const_pointer data() const noexcept { return address_.data() ; }

	constexpr pointer begin() noexcept { return address_.data() ; }
	constexpr pointer end() noexcept { return address_.data() + capacity ; }
	constexpr const_pointer begin() const noexcept { return address_.data() ; }
	constexpr const_pointer end() const noexcept { return address_.data() + capacity ; }

	constexpr reference front() noexcept { return address_.front() ; }
	constexpr reference back() noexcept { return address_.back() ; }
	constexpr const_reference front() const noexcept { return address_.front() ; }
	constexpr const_reference back() const noexcept { return address_.back() ; }

	constexpr IPv4 operator|(const IPv4& o) const noexcept {
		return IPv4(toUint32() | o.toUint32()) ;
	}

	constexpr IPv4 operator&(const IPv4& o) const noexcept {
		return IPv4(toUint32() & o.toUint32()) ;
	}

	constexpr IPv4 operator^(const IPv4& o) const noexcept {
		return IPv4(toUint32() ^ o.toUint32()) ;
	}

	constexpr IPv4 operator~() const noexcept {
		return IPv4(~toUint32()) ;
	}

	constexpr IPv4& operator|=(const IPv4& o) noexcept {
		*this = *this | o ;
		return *this ;
	}

	constexpr IPv4& operator&=(const IPv4& o) noexcept {
		*this = *this & o ;
		return *this ;
	}

	constexpr IPv4& operator^=(const IPv4& o) noexcept {
		*this = *this ^ o ;
		return *this ;
	}

	constexpr IPv4 operator+(uint32_t value) const noexcept {
		return IPv4(toUint32() + value) ;
	}

	constexpr IPv4 operator-(uint32_t value) const noexcept {
		return IPv4(toUint32() - value) ;
	}

	constexpr IPv4& operator+=(uint32_t value) noexcept {
		*this = *this + value ;
		return *this ;
	}

	constexpr IPv4& operator-=(uint32_t value) noexcept {
		*this = *this - value ;
		return *this ;
	}

	constexpr IPv4& operator++() noexcept {
		*this += 1u ;
		return *this ;
	}

	constexpr IPv4& operator--() noexcept {
		*this -= 1u ;
		return *this ;
	}

	constexpr IPv4 operator++(int) noexcept {
		auto r = *this ;
		*this += 1u ;
		return r ;
	}

	constexpr IPv4 operator--(int) noexcept {
		auto r = *this ;
		*this -= 1u ;
		return r ;
	}

	static constexpr IPv4 mask(uint8_t prefix) noexcept {
        if (prefix == 0) return IPv4(0u) ;
        if (prefix >= 32) return IPv4(~0u) ;
        return IPv4((~uint32_t{0}) << (32 - std::clamp(prefix, uint8_t{0}, uint8_t{32}))) ;
    }
} ;

} // namespace ZHTTP