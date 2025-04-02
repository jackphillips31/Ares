/**
 * @file HashSet.h
 * @brief Defines a convenient alias for an EASTL hash set container.
 * @details This file provides a type alias for an unordered set, simplifying usage
 * and allowing for flexibility in changing the underlying implementation.
 * Using this alias ensures consistency in data structures used for fast lookups.
 * 
 * @ingroup aliases
 * @ingroup containers
 */
#pragma once
#include <EASTL/hash_set.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef HashSet
	 * @brief Alias for an `eastl::hash_set`.
	 * 
	 * @details Provides a shorthand for `eastl::hash_set`, improving readability
	 * and maintainability in scenarios requiring quick element lookups.
	 * 
	 * @tparam T Type of elements stored in the set.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type, bool cacheHashCode = false>
	using HashSet = eastl::hash_set<Type, eastl::hash<Type>, eastl::equal_to<Type>, Internal::AppAllocator, cacheHashCode>;

}