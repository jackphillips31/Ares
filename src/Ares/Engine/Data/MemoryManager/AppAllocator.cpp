#include <arespch.h>
#include "Engine/Data/MemoryManager/AppAllocator.h"

#include "Engine/Core/Application.h"
#include "Engine/Data/MemoryManager.h"

namespace Ares::Internal {

	AppAllocator::AppAllocator()
		: m_Manager(nullptr)
	{
	#if AR_BUILD_DEBUG
		m_Name = nullptr;
	#endif
		m_Manager = &Ares::Application::Get().GetMemoryManager();
		if (!m_Manager)
		{
			AR_CORE_ASSERT(false, "Tried to instantiate Allocator without a MemoryManager!");
			throw std::runtime_error("Tried to instantiate Allocator without a MemoryManager!");
		}
	}

	EASTL_ALLOCATOR_EXPLICIT AppAllocator::AppAllocator(const char* name)
		: m_Manager(nullptr)
	{
	#if AR_BUILD_DEBUG
		m_Name = name;
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
	#if AR_BUILD_DEBUG
		m_Name = nullptr;
	#endif
	}

	AppAllocator::AppAllocator(const AppAllocator& other)
		: m_Manager(other.m_Manager)
	{
	#if AR_BUILD_DEBUG
		m_Name = other.m_Name;
	#endif
	}

	AppAllocator::AppAllocator(const AppAllocator& other, const char* name)
		: m_Manager(other.m_Manager)
	{
	#if AR_BUILD_DEBUG
		m_Name = name;
	#endif
	}

	AppAllocator& AppAllocator::operator=(const AppAllocator& other)
	{
		m_Manager = other.m_Manager;

	#if AR_BUILD_DEBUG
		m_Name = other.m_Name;
	#endif

		return *this;
	}

	void* AppAllocator::allocate(size_t size, int flags) const
	{
		//AR_CORE_TRACE("APP ALLOCATION: {}", size);
		AR_CORE_ASSERT(m_Manager != nullptr, "Tried to use Allocator without MemoryManager!");
		if (!m_Manager)
			throw std::runtime_error("Allocator does not have MemoryManager instance!");
		return m_Manager->Allocate(size);
	}

	void* AppAllocator::allocate(size_t size, size_t alignment, size_t offset, int flags) const
	{
		//AR_CORE_TRACE("APP ALLOCATION: {}", size);
		AR_CORE_ASSERT(m_Manager != nullptr, "Tried to use Allocator without MemoryManager");
		if (!m_Manager)
			throw std::runtime_error("Allocator does not have MemoryManager instance!");
		return m_Manager->Allocate(size, alignment, offset);
	}

	void AppAllocator::deallocate(void* ptr, size_t size) const
	{
		AR_CORE_ASSERT(m_Manager != nullptr, "Tried to use Allocator without MemoryManager!");
		if (!m_Manager)
			throw std::runtime_error("Allocator does not have MemoryManager instance!");
		return m_Manager->Deallocate(ptr);
	}

	const char* AppAllocator::get_name() const
	{
	#if AR_BUILD_DEBUG
		return m_Name;
	#endif
		return nullptr;
	}

	void AppAllocator::set_name(const char* name)
	{
	#if AR_BUILD_DEBUG
		m_Name = name;
	#endif
	}

}

namespace eastl {

	Ares::Internal::AppAllocator* get_default_allocator(const Ares::Internal::AppAllocator*)
	{
		return Ares::Internal::GetDefaultAllocator();
	}

}