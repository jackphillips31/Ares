#include <arespch.h>

#include <EASTL/allocator.h>

#include "Engine/Core/Core.h"

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	if (void* ptr = new(0, 0, pName, flags, debugFlags, file, line) char[size])
	{
		return ptr;
	}
	throw std::bad_alloc();
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	// Default alignment to AR_PLATFORM_PTR_SIZE if zero
	size_t adjustedAlignment = (alignment > 0) ? alignment : AR_PLATFORM_MIN_MALLOC_ALIGNMENT;

	// Ensure alignment meets the minimum requirement
	adjustedAlignment = (adjustedAlignment < AR_PLATFORM_MIN_MALLOC_ALIGNMENT) ? AR_PLATFORM_MIN_MALLOC_ALIGNMENT : adjustedAlignment;

	// Validate that alignment is a power of two
	if ((adjustedAlignment & (adjustedAlignment - 1)) != 0)
	{
		AR_CORE_ASSERT(false, "Alignment must be a power of two!");
		throw std::invalid_argument("Alignment must be a power of two!");
	}

	// Validate alignmentOffset (it must be less than alignment)
	if (alignmentOffset >= adjustedAlignment)
	{
		AR_CORE_ASSERT(false, "Alignment offset must be less than alignment!");
		throw std::invalid_argument("Alignment offset must be less than alignment!");
	}

	// Allocate memory with space for alignment and metadata storage
	void* ptr = malloc(size + adjustedAlignment + AR_PLATFORM_PTR_SIZE);
	if (!ptr)
	{
		AR_CORE_ASSERT(false, "Memory allocation failed!");
		throw std::bad_alloc();
	}

	// Adjust pointer for alignment and offset
	void* ptrPlusPtrSize = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + AR_PLATFORM_PTR_SIZE + alignmentOffset);
	void* ptrAligned = reinterpret_cast<void*>(
		(reinterpret_cast<uintptr_t>(ptrPlusPtrSize) + adjustedAlignment - 1) & ~(adjustedAlignment - 1)
	);

	// Store the original pointer just before the aligned pointer
	void** ptrStoredPtr = reinterpret_cast<void**>(ptrAligned) - 1;

	if (ptrStoredPtr < ptr)
	{
		AR_CORE_ASSERT(false, "Aligned pointer offset exceeds allocated memory range!");
		std::free(ptr);
		throw std::invalid_argument("Aligned pointer offset exceeds allocated memory range!");
	}

	*ptrStoredPtr = ptr;

	// Ensure the aligned pointer meets the alignment requirement
	if ((reinterpret_cast<size_t>(ptrAligned) & (adjustedAlignment - 1)) != 0)
	{
		AR_CORE_ASSERT(false, "Aligned pointer does not meet alignment requirement!");
		std::free(ptr);
		throw std::invalid_argument("Aligned pointer does not meet alignment requirement!");
	}

	return ptrAligned;
}

void operator delete[](void* ptr) noexcept
{
	if (!ptr) return;

	// Retrieve the original pointer stored just before the aligned pointer
	void** ptrStoredPtr = reinterpret_cast<void**>(ptr) - 1;
	void* originalPtr = *ptrStoredPtr;

	// Free the original pointer
	std::free(originalPtr);
}