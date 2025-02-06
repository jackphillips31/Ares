#include <arespch.h>
#include "Engine/Data/MemoryManager.h"

namespace Ares::Internal {

	MemoryManager::MemoryManager()
		: m_SelfPool(1024 * 1024), m_SelfAllocator(this), m_PtrPoolMap(m_SelfAllocator), m_Pools(m_SelfAllocator)
	{
		m_Pools.set_allocator(m_SelfAllocator);
		m_Pools.emplace_back(MemoryPool(1024 * 4));

		m_DefaultAllocator = new (m_SelfAllocator.allocate(sizeof(AppAllocator))) AppAllocator(this);
	}

	MemoryManager::~MemoryManager()
	{
		m_SelfAllocator.deallocate(m_DefaultAllocator);
	}

	void* MemoryManager::Allocate(size_t size, int flags)
	{
		size_t poolSize = m_Pools.size();
		for (size_t i = 0; i < m_Pools.size(); i++)
		{
			void* result = m_Pools[i].Allocate(static_cast<uint32_t>(size));
			if (result != nullptr)
			{
				AR_CORE_ASSERT(result != nullptr, "Something went wrong when allocating!");
				m_PtrPoolMap[reinterpret_cast<uintptr_t>(result)] = i;
				return result;
			}
		}

		// Create new pool
		m_Pools.emplace_back(MemoryPool(1024 * 4));
		void* result = m_Pools[poolSize].Allocate(static_cast<uint32_t>(size));
		AR_CORE_ASSERT(result != nullptr, "Something went wrong when creating a new pool!");
		m_PtrPoolMap[reinterpret_cast<uintptr_t>(result)] = poolSize;
		return result;
	}

	void* MemoryManager::Allocate(size_t size, size_t alignment, size_t offset, int flags)
	{
		size_t poolSize = m_Pools.size();
		for (size_t i = 0; i < m_Pools.size(); i++)
		{
			void* result = m_Pools[i].Allocate(static_cast<uint32_t>(size), static_cast<uint32_t>(alignment), static_cast<uint32_t>(offset));
			if (result != nullptr)
			{
				AR_CORE_ASSERT(result != nullptr, "Something went wrong when allocating!");
				m_PtrPoolMap[reinterpret_cast<uintptr_t>(result)] = i;
				return result;
			}
		}

		// Create new pool
		m_Pools.emplace_back(MemoryPool(1024 * 4));
		void* result = m_Pools[poolSize].Allocate(static_cast<uint32_t>(size), static_cast<uint32_t>(alignment), static_cast<uint32_t>(offset));
		AR_CORE_ASSERT(result != nullptr, "Something went wrong when creating a new pool!");
		m_PtrPoolMap[reinterpret_cast<uintptr_t>(result)] = poolSize;
		return result;
	}

	void MemoryManager::Deallocate(void* ptr, size_t size)
	{
		if (!ptr)
			return;

		auto it = m_PtrPoolMap.find(reinterpret_cast<uintptr_t>(ptr));
		if (it != m_PtrPoolMap.end())
		{
			m_Pools[it->second].Deallocate(ptr);
			m_PtrPoolMap.erase(it);
		}
	}

	void* MemoryManager::SelfAllocate(size_t size)
	{
		return m_SelfPool.Allocate(static_cast<uint32_t>(size));
	}

	void* MemoryManager::SelfAllocate(size_t size, size_t alignment, size_t offset)
	{
		return m_SelfPool.Allocate(static_cast<uint32_t>(size), static_cast<uint32_t>(alignment), static_cast<uint32_t>(offset));
	}

	void MemoryManager::SelfDeallocate(void* ptr, size_t size)
	{
		m_SelfPool.Deallocate(ptr);
	}

}