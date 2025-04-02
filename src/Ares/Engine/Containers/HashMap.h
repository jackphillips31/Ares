/**
 * @file HashMap.h
 * @brief Defines a convenient alias for an EASTL hash map container.
 * @details This file provides a type alias for a hash map, simplifying usage
 * and allowing for future flexibility in choosing the underlying container.
 * By using this alias, the engine can easily switch to a different hash map
 * implementation if performance improvements are needed.
 * 
 * @ingroup aliases
 * @ingroup containers
 */
#pragma once
#include <EASTL/hash_map.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef HashMap
	 * @brief Alias for `eastl::hash_map`.
	 * 
	 * @details Features:
	 * @li Uses [AppAllocator](#Ares::Internal::AppAllocator) for memory management.
	 * @li 30-50% faster iteration that std::unordered_map
	 * @li Custom hash/equality function via EASTL
	 * 
	 * @tparam Type1 Key type (must be hashable).
	 * @tparam Type2 Value type.
	 * @tparam cacheHashCode When `true`, stores hash codes for faster rehashing (default: `false`).
	 * 
	 * @note Enable `cacheHashCode` for complex key types (e.g. strings).
	 * @ingroup aliases
	 */
	template <typename Type1, typename Type2, bool cacheHashCode = false>
	using HashMap = eastl::hash_map<Type1, Type2, eastl::hash<Type1>, eastl::equal_to<Type1>, Internal::AppAllocator, cacheHashCode>;

}