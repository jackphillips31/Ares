/**
 * @file HashSet.h
 * @brief Defines a convenient alias for an EASTL hash set container, optimized for fast lookup and efficient memory usage.
 * 
 * @details This file provides a type alias for `eastl::hash_set`, a hash-based unordered set container that
 * offers high-performance element lookup and insertion. It supports customizable memory allocator and
 * optional hash code caching to improve rehashing efficiency.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/hash_set.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef HashSet
	 * @brief Alias for an `eastl::hash_set`, optimized for fast element lookup and customizable memory allocation.
	 * 
	 * @details **Features**:
	 * @li Uses a customizable allocator, defaulting to [AppAllocator](#Ares::Internal::AppAllocator).
	 * @li **Faster lookups** than `std::unordered_set` due to EASTL optimizations.
	 * @li Supports custom hash and equality functions via EASTL.
	 * @li Optional **hash code caching** for faster rehashing of expensive key types.
	 * 
	 * **Performance Characteristics**:
	 * @li **Cache Hash Code** - Performance is significantly improved when `cacheHashCode` is enabled (for complex types).
	 * 
	 * @tparam Type Element type (must be hashable or have a `eastl::hash<Type>` specialization).
	 * @tparam cacheHashCode When `true`, stores hash codes for faster rehashing (default: `false`).
	 * @tparam Allocator Memory allocator type (default: [AppAllocator](#Ares::Internal::AppAllocator)).
	 * 
	 * @note Enable `cacheHashCode` for complex key types (e.g., strings) to reduce hash recomputation overhead.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type, bool cacheHashCode = false, typename Allocator = Internal::AppAllocator>
	using HashSet = eastl::hash_set<Type, eastl::hash<Type>, eastl::equal_to<Type>, Allocator, cacheHashCode>;

}