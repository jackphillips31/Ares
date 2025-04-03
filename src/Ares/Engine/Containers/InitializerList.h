/**
 * @file InitializerList.h
 * @brief Defines a convenient alias for an initializer list, simplifying uniform initialization syntax.
 * 
 * @details This file provides a type alias for `std::initializer_list`, which is used for convenient
 * initialization of containers and aggregates in a uniform and readable manner.
 */
#pragma once

namespace Ares {

	/**
	 * @typedef InitializerList
	 * @brief Alias for `std::initializer_list`, providing convenient list-style initialization.
	 * 
	 * @details **Features**:
	 * @li Enables **uniform initialization** for containers and aggregates.
	 * @li Provides **read-only** access to a sequence of elements.
	 * @li Often used in **constructor initialization lists** for collection types.
	 * @li Works seamlessly with **C++ range based for loops**.
	 * 
	 * @tparam Type Element type stored in the initializer list.
	 * 
	 * @note `std::initializer_list` is a lightweight wrapper for a C-style array with a known size at compile time.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type>
	using InitializerList = std::initializer_list<Type>;

}