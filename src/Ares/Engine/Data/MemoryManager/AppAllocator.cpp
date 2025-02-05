#include <arespch.h>
#include "Engine/Data/MemoryManager/AppAllocator.h"

#include "Engine/Core/Application.h"
#include "Engine/Data/MemoryManager.h"

namespace Ares::Internal {

	AppAllocator::AppAllocator()
		: m_Manager(nullptr)
	{
		m_Manager = &Ares::Application::Get().GetMemoryManager();
		if (!m_Manager)
		{
			AR_CORE_ASSERT(false, "Tried to instantiate Allocator without a MemoryManager!");
			throw std::runtime_error("Tried to instantiate Allocator without a MemoryManager!");
		}
	}

	EASTL_ALLOCATOR_EXPLICIT AppAllocator::AppAllocator(const char* pName)
		: m_Manager(nullptr)
	{
#if EASTL_NAME_ENABLED
		m_pName = pName;
#endif
		m_Manager = &Ares::Application::Get().GetMemoryManager();
		if (!m_Manager)
		{
			AR_CORE_ASSERT(false, "Tried to instantiate Allocator without a MemoryManager!");
			throw std::runtime_error("Tried to instantiate Allocator without a MemoryManager!");
		}
	}

	EASTL_ALLOCATOR_EXPLICIT AppAllocator::AppAllocator(MemoryManager* manager)
		: m_Manager(manager)
	{
	}
	EASTL_ALLOCATOR_EXPLICIT AppAllocator::AppAllocator(MemoryManager& manager)
		: m_Manager(&manager)
	{
	}

	AppAllocator::AppAllocator(const AppAllocator& other)
		: m_Manager(other.m_Manager)
	{
	}

	AppAllocator::AppAllocator(const AppAllocator& other, const char* pName)
		: m_Manager(other.m_Manager)
	{
#if EASTL_NAME_ENABLED
		m_pName = pName;
#endif
	}

	AppAllocator& AppAllocator::operator=(const AppAllocator& other)
	{
		m_Manager = other.m_Manager;
		return *this;
	}

	void* AppAllocator::allocate(size_t size, int flags) const
	{
		AR_CORE_ASSERT(m_Manager, "Tried to use Allocator without MemoryManager!");
		if (!m_Manager)
			throw std::runtime_error("Allocator does not have MemoryManager instance!");
		return m_Manager->Allocate(size);
	}

	void* AppAllocator::allocate(size_t size, size_t alignment, size_t offset, int flags) const
	{
		AR_CORE_ASSERT(m_Manager, "Tried to use Allocator without MemoryManager");
		if (!m_Manager)
			throw std::runtime_error("Allocator does not have MemoryManager instance!");
		return m_Manager->Allocate(size, alignment, offset);
	}

	void AppAllocator::deallocate(void* ptr, size_t size) const
	{
		AR_CORE_ASSERT(m_Manager, "Tried to use Allocator without MemoryManager!");
		if (!m_Manager)
			throw std::runtime_error("Allocator does not have MemoryManager instance!");
		return m_Manager->Deallocate(ptr);
	}

	const char* AppAllocator::get_name() const
	{
#if EASTL_NAME_ENABLED
		return m_pName;
#endif
		return nullptr;
	}

	void AppAllocator::set_name(const char* pName)
	{
#if EASTL_NAME_ENABLED
		m_pName = pName;
#endif
	}

	AppAllocator* get_default_allocator(const AppAllocator*)
	{
		return ::Ares::Application::Get().GetMemoryManager().GetDefaultAllocator();
	}

	bool operator==(const AppAllocator& a, const AppAllocator& b)
	{
		return a.m_Manager == b.m_Manager;
	}

	bool operator!=(const AppAllocator& a, const AppAllocator& b)
	{
		return a.m_Manager != b.m_Manager;
	}

}