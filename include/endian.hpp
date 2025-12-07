#pragma once

#include <bit>
#include <concepts>

namespace ZHTTP {

	static constexpr bool IS_LITTLE_ENDIAN = (std::endian::native == std::endian::little) ;
	static constexpr bool IS_BIG_ENDIAN = (std::endian::native == std::endian::big) ;

} // namespace ZHTTP