/**
 * @file Atomic.h
 * @brief Defines a convenient alias for an EASTL atomic object.
 * @details This file provides a type alias for `eastl::atomic`, which allows atomic
 * operation on the specified type. This alias is useful for ensuring safe concurrent
 * access to variables, especially in multi-threaded environments.
 * 
 * @ingroup aliases
 * @ingroup multithreading
 */
#pragma once
#include <EASTL/atomic.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef Atomic
	 * @brief Alias for `eastl::atomic` to provide atomic operations.
	 * 
	 * @details **Features**:
	 * @li Uses `eastl::atomic<Type>` to provide atomic operations on the specified type.
	 * @li Supports atomic load, store, fetch-add, and compare-exchange operations.
	 * @li Ensures safe concurrent access to shared data in multi-threaded applications.
	 * 
	 * |Feature|Description|
	 * |-|-|
	 * |**Atomic Operations**|Provides atomic operations such as load, store, fetch-add, and CAS.|
	 * |**Memory Ordering**|Supports different memory ordering options (e.g., relaxed, aquire, release).|
	 * |**Concurrency Safety**|Ensures safe concurrent access in multi-threaded applications.|
	 * |**Performance**|Lightweight compared to manual synchronization methods like mutexes.|
	 * |**Cache Efficiency**|Operations are optimized for cache-line alignment to avoid false sharing.|
	 * 
	 * @note The Atomic alias is designed for safe usage in multi-threaded environments.
	 * It ensures that operations on the variable are performed atomically, preventing
	 * race conditions.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type>
	using Atomic = eastl::atomic<Type>;

}