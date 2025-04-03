/**
 * @file HashMap.h
 * @brief Defines a convenient alias for an EASTL hash map container, optimized for performance and efficient memory usage.
 * 
 * @details This file provides a type alias for `eastl::hash_map`, offering a hash-based
 * associative container with improved iteration performance and flexible allocator support.
 * The alias simplifies usage while allowing optional caching of hash codes for faster rehashing.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/hash_map.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef HashMap
	 * @brief Alias for `eastl::hash_map`, optimized for fast iteration and custom memory allocation.
	 * 
	 * @details **Features**:
	 * @li Uses a customizable allocator, defaulting to [AppAllocator](#Ares::Internal::AppAllocator).
	 * @li **30-50% faster iteration** compared to `std::unordered_map`.
	 * @li Supports custom hash and equality function via EASTL.
	 * @li Optional **hash code caching** for faster rehashing when using complex key types.
	 * 
	 * **Performance Characteristics**:
	 * @li **Cache Hash Code** - Performance is significantly improved when `cacheHashCode` is enabled (for complex types).
	 * 
	 * @tparam Type1 Key type (must be hashable or have a `eastl::hash<Type1>` specialization).
	 * @tparam Type2 Value type.
	 * @tparam cacheHashCode When `true`, stores hash codes for faster rehashing (default: `false`).
	 * @tparam Allocator Memory allocator type (default: [AppAllocator](#Ares::Internal::AppAllocator)).
	 *
	 * @note Enable `cacheHashCode` for complex key types (e.g., strings) to reduce hash recomputation overhead.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type1, typename Type2, bool cacheHashCode = false, typename Allocator = Internal::AppAllocator>
	using HashMap = eastl::hash_map<Type1, Type2, eastl::hash<Type1>, eastl::equal_to<Type1>, Allocator, cacheHashCode>;

}