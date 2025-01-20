#include <arespch.h>
#include "Engine/Core/Allocator.h"

#include <EASTL/allocator.h>

#include "Engine/Core/Core.h"

// Platform-specific implementations of the custom new[] operators
void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	if (debugFlags)
	{
		std::cout << "Allocating " << size << " bytes at " << file << ":" << line << std::endl;
	}

	// Default allocation
	void* ptr = malloc(size);

	// Check if allocation was successful
	if (!ptr)
	{
		throw std::bad_alloc();
	}

	return ptr;
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	if (debugFlags)
	{
		std::cout << "Allocating " << size << " bytes at " << file << ":" << line << std::endl;
	}

	// Check alignment constraints
	if (alignment <= 0 || (alignment & (alignment - 1)) != 0)
	{
		throw std::invalid_argument("Alignment must be a power of 2.");
	}

	void* ptr = nullptr;

#if defined(AR_PLATFORM_WINDOWS)
	// On Windows, use _aligned_malloc for alignment
	ptr = _aligned_malloc(size + alignmentOffset, alignment);
#elif defined(AR_PLATFORM_LINUX) || defined(AR_PLATFORM_MACOS) || defined(AR_PLATFORM_IOS)
	// On POSIX systems, use posix_memalign
	if (posix_memalign(&ptr, alignment, size + alignmentOffset) != 0)
	{
		return nullptr;
	}
#else
	// Default case, fallback to malloc without alignment
	ptr = malloc(size);
#endif

	// Check if allocation was successful
	if (!ptr)
	{
		throw std::bad_alloc();
	}

	// Adjust pointer to meet alignment offset
	if (alignmentOffset > 0)
	{
		void* alignedPtr = static_cast<void*>(reinterpret_cast<char*>(ptr) + alignmentOffset);
		return alignedPtr;
	}

	return ptr;
}

void operator delete[](void* ptr) noexcept
{
	free(ptr);
}

void operator delete[](void* ptr, const char* pName, int flags, unsigned debugFlags, const char* file, int line) noexcept
{
	if (debugFlags)
	{
		AR_CORE_DEBUG("Deallocating memory at {}:{}", file, line);
	}

	operator delete[](ptr);
}

void operator delete[](void* ptr, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) noexcept
{
	if (debugFlags)
	{
		AR_CORE_DEBUG("Deallocating memory at {}:{}", file, line);
	}

#if defined(AR_PLATFORM_WINDOWS)
	_aligned_free(ptr);
#elif defined(AR_PLATFORM_LINUX) || defined(AR_PLATFORM_MACOS) || defined(AR_PLATFORM_IOS)
	free(ptr);
#else
	free(ptr);
#endif
}