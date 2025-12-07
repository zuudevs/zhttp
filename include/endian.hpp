#pragma once

#include <bit>
#include <concepts>

namespace ZHTTP {

	static constexpr bool IS_LITTLE_ENDIAN = (std::endian::native == std::endian::little) ;
	static constexpr bool IS_BIG_ENDIAN = (std::endian::native == std::endian::big) ;

	constexpr inline auto ENDIAN_SWAP(std::unsigned_integral auto value) noexcept -> decltype(value) {
		return IS_LITTLE_ENDIAN ? std::byteswap(value) : value ;
	}

} // namespace ZHTTP