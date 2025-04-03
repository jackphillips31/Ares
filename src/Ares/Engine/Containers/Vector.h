/**
 * @file Vector.h
 * @brief Defines an alias for an EASTL vector container.
 * 
 * @details This file defines [Vector](#Ares::Vector), an alias for `eastl::vector`,
 * providing a dynamic array container with fast random access and automatic memory
 * management. It is optimized for performance while maintaining ease of use.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/vector.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	/**
	 * @typedef Vector
	 * @brief Alias for `eastl::vector`, a dynamic array container.
	 * 
	 * @details **Features**:
	 * @li Provides **fast random access** to elements.
	 * @li Supports **efficient dynamic resizing**.
	 * @li Uses **custom memory allocators** via `Allocator` parameter (default: [AppAllocator](#Ares::Internal::AppAllocator)).
	 * @li **Optimized for performance** in high-performance applications.
	 * 
	 * @tparam Type The type of elements to be stored in the vector.
	 * @tparam Allocator The allocator used for memory management (default: [AppAllocator](#Ares::Internal::AppAllocator)).
	 * 
	 * @note While [Vector](#Ares::Vector) provides dynamic resizing, frequent insertions in the middle
	 * may be inefficient compared to linked lists or deques.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type, typename Allocator = Internal::AppAllocator>
	using Vector = eastl::vector<Type, Allocator>;

}