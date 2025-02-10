#pragma once
#include "Engine/Data/MemoryManager/AVLTree.h"

namespace Ares::Internal {

	class MemoryPool
	{
	public:
		MemoryPool(const size_t& size);
		~MemoryPool();

		MemoryPool(const MemoryPool&) = delete;
		MemoryPool& operator=(const MemoryPool&) = delete;

		MemoryPool(MemoryPool&& other) noexcept;
		MemoryPool& operator=(MemoryPool&& other) noexcept;

		void* Allocate(const uint32_t& size);
		void* Allocate(const uint32_t& size, const uint32_t& alignment, const uint32_t& alignmentOffset);

		void Deallocate(void* ptr);

	private:
		uint32_t AdjustSize(const uint32_t& size) const;
		uint32_t AdjustSize(const uint32_t& size, const uint32_t& alignment, const uint32_t& offset) const;
		uint32_t AdjustAlignment(const uint32_t& alignment) const;

	private:
		void* m_Data;
		void* m_Head;
		uint32_t m_Size;
		uintptr_t m_PoolStart;
		std::shared_mutex m_PoolMutex;
	};

	struct AVLNodeData;
	class MemoryBlockNew;

	class MemoryPoolNew
	{
	public:
		MemoryPoolNew(size_t poolSize);
		MemoryPoolNew(const MemoryPoolNew&) = delete;
		MemoryPoolNew& operator=(const MemoryPoolNew&) = delete;
		MemoryPoolNew(MemoryPoolNew&& other) noexcept;
		MemoryPoolNew& operator=(MemoryPoolNew&& other) noexcept;
		~MemoryPoolNew();

		void* Allocate(const size_t& size);
		void* Allocate(const size_t& size, const size_t& alignment, const size_t& offset);
		void Deallocate(void* ptr, const size_t& size = 0);

	private:
		void* m_Data;
		void* m_PoolStart;
		size_t m_Size;
		size_t m_PoolSize;
		AVLTree m_AVLTree;

		std::shared_mutex m_Mutex;
	};

}