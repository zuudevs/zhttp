#pragma once

/**
 * @file util/endian.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief 
 * @version 1.0.0
 * @date 2025-12-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <bit>
#include <concepts>

namespace frqs {

	static constexpr bool IS_LITTLE_ENDIAN = (std::endian::native == std::endian::little) ;
	static constexpr bool IS_BIG_ENDIAN = (std::endian::native == std::endian::big) ;

} // namespace frqs
