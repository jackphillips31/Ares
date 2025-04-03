/**
 * @file Variant.h
 * @brief Defines the Variant alias for type-safe unions.
 * 
 * @details This file defines [Variant](#Ares::Variant), providing a type-safe,
 * discriminated union that can hold one of multiple types at a time. It is useful
 * for dynamic type storage while maintaining strong type safety.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/variant.h>

namespace Ares {

	/**
	 * @typedef Variant
	 * @brief Alias for `eastl::variant`, a type-safe, discriminated union.
	 * 
	 * @details **Features**:
	 * @li Holds **one of multiple types** at any given time.
	 * @li Uses **compile-time type checking** to prevent invalid access.
	 * @li Compatible with `eastl::visit` for structured type handling.
	 * @li Optimized for **fast access and minimal overhead**.
	 * 
	 * @tparam Types... The list of types [Variant](#Ares::Variant) can store.
	 * 
	 * @note [Variant](#Ares::Variant) is more flexible than a union but may have a larger memory footprint.
	 * 
	 * @ingroup aliases
	 */
	template <class... Types>
	using Variant = eastl::variant<Types...>;

}