/**
 * @file InitializerList.h
 * @brief Defines a convenient alias for an initializer list.
 * @details This file provides a type alias for `std::initializer_list`, allowing
 * for easier usage in contexts where a list of elements is needed,
 * such as in initialization of containers or arguments.
 * Using this alias improves code readability and ensures consistency
 * across the engine or application.
 * 
 * @ingroup aliases
 */
#pragma once

namespace Ares {

	/**
	 * @typedef InitializerList
	 * @brief Alias for `std::initializer_list`
	 * 
	 * @details Provides a shorthand for `std::initializer_list`, simplifying its usage
	 * in cases where a list of elements needs to be passed to constructors
	 * or functions.
	 * 
	 * @tparam T Type of elements in the initializer list.
	 * 
	 * @ingroup aliases
	 */
	template <class E>
	using InitializerList = std::initializer_list<E>;

}