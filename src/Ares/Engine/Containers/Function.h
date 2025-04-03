/**
 * @file Function.h
 * @brief Defines a convenient alias for EASTL function objects.
 * @details This file provides a type alias for `eastl::function`, which allows the creation of
 * type-safe function objects that can store and invoke callable objects such as
 * regular functions, function pointers, or lambdas. This alias simplifies the usage
 * of `eastl::function` for storing and invoking functions with various signatures.
 */
#pragma once
#include <EASTL/functional.h>

namespace Ares {

	/**
	 * @typedef Function
	 * @brief Alias for `eastl::function` to store callable objects.
	 * 
	 * @details **Features**:
	 * @li Uses `eastl::function<T>` to store and invoke callable objects.
	 * @li Supports various callable types, including regular functions, function pointers,
	 * and lambdas.
	 * @li Type-safe and flexible, allowing different signatures to be handled.
	 * 
	 * **Performance Characteristics**:
	 * @li **Overhead** - May introduce some overhead due to type erause (comparable to std::function).
	 * @li **Memory Usage** - Slightly higher memory usage compared to plain function pointers.
	 * 
	 * @tparam Type The type of callable object (e.g., a function signature).
	 * 
	 * @note The Function alias is designed to simplify the usage of `eastl::function` by
	 * providing a clean interface for working with type-erased callable objects.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type>
	using Function = eastl::function<Type>;

}