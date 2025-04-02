/**
 * @file Queue.h
 * @brief Defines a convenient alias for the EASTL queue container.
 * @details This file provides a type alias for `eastl::queue`, allowing for more readable
 * and concise code when working with queues. Using this alias can improve
 * maintainability and ensure consistency across the engine or application,
 * especially when working with FIFO (First-In-First-Out) data structures.
 * 
 * @ingroup aliases
 * @ingroup containers
 */
#pragma once
#include <EASTL/queue.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef Queue
	 * @brief Alias for `eastl::queue` using `eastl::deque` as the underlying container.
	 * 
	 * @details **Features**:
	 * @li Uses a customizable allocator, defaulting to [AppAllocator](#Ares::Internal::AppAllocator),
	 * optimizing memory allocation for high-performance applications.
	 * @li The underlying container is `eastl::deque<Type, Allocator>`, which supports
	 * efficient FIFO operations.
	 * @li Provides better memory control and performance compared to `std::queue`.
	 * 
	 * **Performance Characteristics**:
	 * |Feature|Description|
	 * |-|-|
	 * |**Allocator**|Uses a customizable allocator, defaulting to[AppAllocator](#Ares::Internal::AppAllocator).|
	 * |**Container**|Uses `eastl::deque` as the underlying container for efficient FIFO.|
	 * |**Performance**|Better iteration performance compared to `std::queue`.|
	 * |**Memory Control**|Fine-grained control over memory allocation due to custom allocator.|
	 * |**Reallocation**|Reduced reallocations during queue operations.|
	 * |**Thread-Safety**|Safe to use in multi-threaded environments with careful synchronization.|
	 * 
	 * @tparam T Type of elements stored in the queue.
	 * @tparam Allocator The allocator type used for memory management (defaults to [AppAllocator](#Ares::Internal::AppAllocator)).
	 * 
	 * @note This alias offers a highly customizable queue implementation that
	 * allows the use of different allocators, with [AppAllocator](#Ares::Internal::AppAllocator)
	 * being the default.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type, typename Allocator = Internal::AppAllocator>
	using Queue = eastl::queue<Type, eastl::deque<Type, Allocator>>;

}