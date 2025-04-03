/**
 * @file Queue.h
 * @brief Defines a convenient alias for the EASTL queue container, a FIFO (First-In, First-Out) container.
 * 
 * @details This file provides a type alias for `eastl::queue`, which provides an efficient
 * queue implementation using `eastl::deque` as the underlying container.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/queue.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef Queue
	 * @brief Alias for `eastl::queue`, a FIFO (First-In, First-Out) container.
	 * 
	 * @details **Features**:
	 * @li Provides **constant time** push and pop operations.
	 * @li Backed by `eastl::deque` for dynamic resizing.
	 * @li Uses [AppAllocator](#Ares::Internal::AppAllocator) for memory management
	 * @li Supports **custom allocators** for specialized memory control.
	 * 
	 * **Performance Characteristics**:
	 * @li **O(1)** - Operations are O(1).
	 * 
	 * @tparam Type The type of elements stored in the queue.
	 * @tparam Allocator The memory allocator used (default: [AppAllocator](#Ares::Internal::AppAllocator)).
	 * 
	 * @note Unlike `std::queue`, this version supports EASTL-specific optimizations.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type, typename Allocator = Internal::AppAllocator>
	using Queue = eastl::queue<Type, eastl::deque<Type, Allocator>>;

}