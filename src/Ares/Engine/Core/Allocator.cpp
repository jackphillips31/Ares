#include <arespch.h>
#include <EASTL/allocator.h>
#include "Engine/Core/Core.h"

#if AR_PLATFORM_WINDOWS
#include <malloc.h>
#define AR_ALIGNED_MALLOC(size, alignment) _aligned_malloc(size, alignment);
#define AR_ALIGNED_FREE(ptr) _aligned_free(ptr);
#elif AR_PLATFORM_POSIX
#include <malloc.h>

inline void* PosixAlignedMalloc(size_t size, size_t alignment)
{
	void* ptr = nullptr;
	if (memalign(&ptr, alignment, size) != 0)
	{
		throw std::bad_alloc();
	}
	return ptr;
}
#define AR_ALIGNED_MALLOC(size, alignment) PosixAlignedMalloc(size, alignment);
#define AR_ALIGNED_FREE(ptr) free(ptr);
#else
#error "Platform not supported!"
#endif

struct AllocationHeader
{
	bool IsAligned;
	void* OriginalPtr;
	#if AR_BUILD_DEBUG
		void* UserPtr;
	#endif
};

namespace eastl {

	allocator::allocator(const char* EASTL_NAME(pName))
	{
		#if EASTL_NAME_ENABLED
			mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
		#endif
	}

	allocator::allocator(const allocator& EASTL_NAME(alloc))
	{
		#if EASTL_NAME_ENABLED
			mpName = alloc.mpName;
		#endif
	}

	allocator::allocator(const allocator& other, const char* EASTL_NAME(pName))
	{
		#if EASTL_NAME_ENABLED
			mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
		#endif
	}

	void* allocator::allocate(size_t size, int flags)
	{
		AR_CORE_TRACE("ALLOCATION: {}", size);
		constexpr size_t headerSize = sizeof(AllocationHeader);

		// Calculate total size
		const bool isBig = size > AR_CACHE_LINE_SIZE;
		size_t totalSize = size + headerSize;
		if (isBig)
		{
			totalSize += AR_CACHE_LINE_SIZE;
		}

		// Allocate memory
		void* originalPtr = malloc(totalSize);
		if (!originalPtr)
		{
			AR_CORE_ASSERT(false, "Memory allocation failure!");
			throw std::bad_alloc();
		}

		AllocationHeader* header = nullptr;
		if (isBig)
		{
			// Place header at the next cache line after user data
			header = reinterpret_cast<AllocationHeader*>((reinterpret_cast<uintptr_t>(originalPtr) + size + AR_CACHE_LINE_SIZE - 1) & ~(AR_CACHE_LINE_SIZE - 1));
		}
		else
		{
			// Place the header at the end of user memory data
			header = reinterpret_cast<AllocationHeader*>(static_cast<char*>(originalPtr) + size);
		}

		header->IsAligned = false;
		header->OriginalPtr = originalPtr;

		#if AR_BUILD_DEBUG
			header->UserPtr = originalPtr;

			#if AR_BUILD_DEBUG && AR_DEBUG_PRINT_ALLOCATIONS
				AR_CORE_DEBUG("[{:>18}] - ALLOCATION            - Size: [{:>8}]", fmt::format("{:#018x}", reinterpret_cast<uintptr_t>(originalPtr)), totalSize);
			#endif
		#endif

		return originalPtr;
	}

	void* allocator::allocate(size_t size, size_t alignment, size_t offset, int flags)
	{
		AR_CORE_TRACE("ALLOCATION: {}", size);
		constexpr size_t headerSize = sizeof(AllocationHeader);

		// Ensure alignment meets the minimum requirement
		size_t adjustedAlignment = (alignment < AR_PLATFORM_MIN_MALLOC_ALIGNMENT) ? AR_PLATFORM_MIN_MALLOC_ALIGNMENT : alignment;

		// Calculate total size
		size_t totalSize = offset + size + headerSize;
		const bool isBig = size > AR_CACHE_LINE_SIZE;
		if (isBig)
		{
			totalSize += AR_CACHE_LINE_SIZE;
		} 

		#if AR_BUILD_DEBUG
			// Validate that alignment is a power of 2
			if ((adjustedAlignment & (adjustedAlignment - 1)) != 0)
			{
				AR_CORE_ASSERT(false, "Alignment must be a power of 2!");
				throw std::invalid_argument("Alignment must be a power of 2!");
			}

			// Validate alignment (it must be greater than or equal to platform minimum)
			if (adjustedAlignment < AR_PLATFORM_MIN_MALLOC_ALIGNMENT)
			{
				AR_CORE_ASSERT(false, "Alignment must be greater than: {}!", AR_PLATFORM_MIN_MALLOC_ALIGNMENT);
				throw std::invalid_argument("Alignment is less than platform's minimum!");
			}
		#endif

		// Allocate memory
		void* originalPtr = AR_ALIGNED_MALLOC(totalSize, adjustedAlignment);
		if (!originalPtr)
		{
			AR_CORE_ASSERT(false, "Memory allocation failure!");
			throw std::bad_alloc();
		}

		// Offset the user pointer
		void* userPtr = static_cast<void*>(static_cast<char*>(originalPtr) + offset);

		AllocationHeader* header = nullptr;
		if (isBig)
		{
			// Place header at the next cache line after user data
			header = reinterpret_cast<AllocationHeader*>((reinterpret_cast<uintptr_t>(userPtr) + size + AR_CACHE_LINE_SIZE - 1) & ~(AR_CACHE_LINE_SIZE - 1));
		}
		else
		{
			// Place the header at the end of user memory data
			header = reinterpret_cast<AllocationHeader*>(static_cast<char*>(userPtr) + size);
		}

		header->IsAligned = true;
		header->OriginalPtr = originalPtr;

		#if AR_BUILD_DEBUG
			header->UserPtr = userPtr;

			#if AR_DEBUG_PRINT_ALLOCATIONS
				if (Ares::Log::GetCoreLogger())
				{
					AR_CORE_DEBUG("[{:>18}] - ALIGNED ALLOCATION    - Size: [{:>8}] - Alignment: [{:>4}]\n",
						fmt::format("{:#018x}", reinterpret_cast<uintptr_t>(userPtr)), totalSize, adjustedAlignment);
				}
			#endif
		#endif

		return userPtr;
	}

	void allocator::deallocate(void* ptr, size_t size)
	{
		constexpr size_t headerSize = sizeof(AllocationHeader);
		if (!ptr) return;

		//AllocationHeader* header = reinterpret_cast<AllocationHeader*>(static_cast<char*>(ptr) + size);
		AllocationHeader* header = nullptr;
		if (size > AR_CACHE_LINE_SIZE)
		{
			header = reinterpret_cast<AllocationHeader*>((reinterpret_cast<uintptr_t>(ptr) + size + AR_CACHE_LINE_SIZE - 1) & ~(AR_CACHE_LINE_SIZE - 1));
		}
		else
		{
			header = reinterpret_cast<AllocationHeader*>(static_cast<char*>(ptr) + size);
		}

		#if AR_BUILD_DEBUG
			if (header->UserPtr != ptr)
			{
				AR_CORE_ASSERT(false, "Memory corruption detected: header user pointer mismatch!");
				return;
			}
		#endif

		if (header->IsAligned)
		{
			AR_ALIGNED_FREE(header->OriginalPtr);
		}
		else
		{
			free(header->OriginalPtr);
		}
	}

	allocator& allocator::operator=(const allocator& EASTL_NAME(alloc))
	{
		#if EASTL_NAME_ENABLED
			mpName = alloc.mpName;
		#endif
		return *this;
	}

	const char* allocator::get_name() const
	{
		#if EASTL_NAME_ENABLED
			return mpName;
		#else
			return EASTL_ALLOCATOR_DEFAULT_NAME;
		#endif
	}

	void allocator::set_name(const char* EASTL_NAME(pName))
	{
		#if EASTL_NAME_ENABLED
			mpName = pName;
		#endif
	}

	allocator gDefaultAllocator;

	allocator* GetDefaultAllocator()
	{
		return &gDefaultAllocator;
	}

}