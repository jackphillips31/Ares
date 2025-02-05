#pragma once
#include "Engine/Data/MemoryManager/AppAllocator.h"
#include "Engine/Data/MemoryManager/Deleter.h"
#include "Engine/Data/MemoryManager/MemoryPool.h"

namespace Ares::Internal {

	class AppAllocator;

	class MemoryManager
	{
	private:
		class SelfAllocator : public eastl::allocator
		{
		public:
			explicit SelfAllocator(MemoryManager* manager) : m_Manager(manager) {}

			inline void* allocate(size_t size, int flags = 0) { return m_Manager->SelfAllocate(size); }
			inline void* allocate(size_t size, size_t alignment, size_t offset, int flags = 0) { return m_Manager->SelfAllocate(size, alignment, offset); }
			inline void deallocate(void* ptr, size_t size = 0) { m_Manager->SelfDeallocate(ptr, size); }

		private:
			MemoryManager* m_Manager;
		};
	public:
		MemoryManager();
		~MemoryManager();

		void* Allocate(size_t size, int flags = 0);
		void* Allocate(size_t size, size_t alignment, size_t offset, int flags = 0);
		void Deallocate(void* ptr, size_t size = 0);

		inline AppAllocator* GetDefaultAllocator() { return m_DefaultAllocator; }

	private:
		void* SelfAllocate(size_t size);
		void* SelfAllocate(size_t size, size_t alignment, size_t offset);
		void SelfDeallocate(void* ptr, size_t size);

	private:
		MemoryPool m_SelfPool;
		SelfAllocator m_SelfAllocator;
		AppAllocator* m_DefaultAllocator;
		eastl::hash_map<uintptr_t, size_t, eastl::hash<uintptr_t>, eastl::equal_to<uintptr_t>, MemoryManager::SelfAllocator> m_PtrPoolMap;
		eastl::vector<MemoryPool, MemoryManager::SelfAllocator> m_Pools;
	};

	//bool operator==(const Allocator& a, const Allocator& b) { return a.GetManager() == b.GetManager(); }
	//bool operator!=(const Allocator& a, const Allocator& b) { return a.GetManager() != b.GetManager(); }

}