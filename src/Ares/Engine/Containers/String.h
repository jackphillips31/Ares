/**
 * @file String.h
 * @brief Defines string type aliases using EASTL.
 * 
 * @details This file defines [BasicString](#Ares::BasicString) and [String](#Ares::String),
 * providing optimized string handling with EASTL's `basic_string`. These aliases offer
 * improved performance and memory control compared to `std::string`, making them suitable
 * for game engine development.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/string.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef BasicString
	 * @brief Alias for `eastl::basic_string`, providing flexible string handling.
	 * 
	 * @details **Features**:
	 * @li Provides **dynamic string storage** with efficient memory management.
	 * @li Uses [AppAllocator](#Ares::Internal::AppAllocator) by default.
	 * @li Supports **custom allocators** for specialized memory control.
	 * @li Compatible with EASTL algorithms for optimized performance.
	 * 
	 * @tparam Type Character type (default: `char`).
	 * @tparam Allocator The memory allocator used (default: [AppAllocator](#Ares::Internal::AppAllocator)).
	 * 
	 * @note Unlike `std::basic_string`, this version is optimized for high-performance applications.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type = char, typename Allocator = Internal::AppAllocator>
	using BasicString = eastl::basic_string<Type, Allocator>;

	/**
	 * @typedef String
	 * @brief Alias for `BasicString<char>`, providing an EASTL-based string type.
	 * 
	 * @details **Features**:
	 * @li Equivalent to [BasicString<char>](#Ares::BasicString), optimized for general string use.
	 * @li Uses [AppAllocator](#Ares::Internal::AppAllocator) by default.
	 * @li Compatible with EASTL utilities and algorithms.
	 * 
	 * @note Replaces `std::string` in performance-critical applications.
	 * 
	 * @ingroup aliases
	 */
	using String = BasicString<char, Internal::AppAllocator>;

}

namespace eastl {

	template<>
	struct hash<Ares::String>
	{
		size_t operator()(const Ares::String& string) const
		{
			return eastl::hash<const char*>()(string.c_str());
		}
	};

}