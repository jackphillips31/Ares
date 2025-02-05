#include <arespch.h>
#include "Engine/Data/MemoryManager/Deleter.h"

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares::Internal {

	Deleter::Deleter()
		: m_Allocator(nullptr)
	{
	}

	Deleter::Deleter(const AppAllocator* alloc)
		: m_Allocator(alloc)
	{
	}

	Deleter::Deleter(const Deleter& other)
		: m_Allocator(other.m_Allocator)
	{
	}

	Deleter& Deleter::operator=(const Deleter& other)
	{
		m_Allocator = other.m_Allocator;
		return *this;
	}

	void Deleter::operator()(void* ptr)
	{
		AR_CORE_ASSERT(m_Allocator, "Deleter does not have an Allocator instance!");
		if (!m_Allocator)
			throw std::runtime_error("Deleter does not have an Allocator instance!");
		m_Allocator->deallocate(ptr);
	}

}