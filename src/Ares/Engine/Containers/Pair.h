/**
 * @file Pair.h
 * @brief Defines a convenient alias for an EASTL pair.
 * @details This file provides a type alias for `eastl::pair`, allowing for more readability
 * and concise code when working with pairs of values. Using this alias can improve
 * maintainability and consistency across the engine or application.
 * 
 * @ingroup aliases
 */
#pragma once
#include <EASTL/utility.h>

namespace Ares {

	/**
	 * @typedef Pair
	 * @brief Alias for `std::pair`.
	 * 
	 * @details Provies a shorthand for `eastl::pair`, making it easier to define and use
	 * pairs of related values. This alias improves readability and simplifies code
	 * when working with pairs of elements.
	 * 
	 * @tparam T1 Type of the first element of the pair.
	 * @tparam T2 Type of the second element of the pair.
	 * 
	 * @ingroup aliases
	 */
	template <typename T1, typename T2>
	using Pair = eastl::pair<T1, T2>;

}