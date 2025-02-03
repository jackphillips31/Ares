#include <arespch.h>
#include "Engine/Data/MemoryManager.h"

#include "Engine/Core/Application.h"

namespace Ares::Internal {

	MemoryManager::MemoryManager()
		: m_SelfPool(1024 * 4), m_SelfAllocator(this), m_DefaultAllocator(this), m_PtrPoolMap(m_SelfAllocator), m_Pools(m_SelfAllocator)
	{
		m_Pools.set_allocator(m_SelfAllocator);
		m_Pools.emplace_back(MemoryPool(1024 * 4));
	}

	MemoryManager::~MemoryManager()
	{
	}

	void* MemoryManager::Allocate(size_t size, int flags)
	{
		size_t poolSize = m_Pools.size();
		for (size_t i = 0; i < m_Pools.size(); i++)
		{
			void* result = m_Pools[i].Allocate(static_cast<uint32_t>(size));
			if (result != nullptr)
			{
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

	MemoryPool::MemoryPool(size_t size)
	{
		size_t totalSize = size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT * 2;
		m_Size = static_cast<uint32_t>(totalSize);

		m_Data = malloc(totalSize);
		if (m_Data)
			std::memset(m_Data, 0, totalSize);
		else
			throw std::bad_alloc();

		uintptr_t dataStart = reinterpret_cast<uintptr_t>(m_Data);
		m_PoolStart =
			dataStart % AR_PLATFORM_MIN_MALLOC_ALIGNMENT == 0 ?
			dataStart :
			(dataStart + AR_PLATFORM_MIN_MALLOC_ALIGNMENT) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		uintptr_t blockEnd = (dataStart + totalSize) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		uint32_t blockSize = static_cast<uint32_t>(blockEnd - m_PoolStart);

		MemoryBlock mainBlock = m_PoolStart;
		mainBlock.SetBlockSize(blockSize);
		mainBlock.SetAlloc(false);
		mainBlock.GetFooter()->SetSize(0);
		mainBlock.GetFooter()->SetAlloc(true);
		mainBlock.SetPrevPtr(nullptr);
		mainBlock.SetNextPtr(nullptr);

		m_Head = mainBlock;
	}

	MemoryPool::~MemoryPool()
	{
		free(m_Data);
	}

	MemoryPool::MemoryPool(MemoryPool&& other) noexcept
		: m_Data(other.m_Data), m_Head(other.m_Head), m_Size(other.m_Size), m_PoolStart(other.m_PoolStart)
	{
		other.m_Data = nullptr;
		other.m_Head = nullptr;
		other.m_Size = 0;
		other.m_PoolStart = 0;
	}

	MemoryPool& MemoryPool::operator=(MemoryPool&& other) noexcept
	{
		m_Data = other.m_Data;
		m_Head = other.m_Head;
		m_Size = other.m_Size;
		m_PoolStart = other.m_PoolStart;

		other.m_Data = nullptr;
		other.m_Head = nullptr;
		other.m_Size = 0;
		other.m_PoolStart = 0;

		return *this;
	}

	void* MemoryPool::Allocate(uint32_t size)
	{
		AR_CORE_TRACE("ALLOCATING: Size - {:10}", size);
		AR_CORE_ASSERT(m_Data, "Cannot use default constructed MemoryPool!");

		uint32_t adjustedSize = AdjustSize(size);

		MemoryBlock currentBlock = m_Head;

		while (currentBlock)
		{
			if (currentBlock.CanFitAndSplit(adjustedSize))
			{
				uint32_t beforeSplitSize = currentBlock.GetBlockSize();
				bool isCurrentLast = currentBlock.IsLastBlock();
				currentBlock.SetPayloadSize(adjustedSize);
				currentBlock.SetAlloc(true);

				MemoryBlock newBlock = reinterpret_cast<char*>(currentBlock.GetFooter()) + 4;
				uint32_t newBlockSize = beforeSplitSize - currentBlock.GetBlockSize();
				newBlock.SetBlockSize(newBlockSize);
				newBlock.SetAlloc(false);

				if (isCurrentLast)
				{
					newBlock.GetFooter()->SetSize(0);
					newBlock.GetFooter()->SetAlloc(true);
				}

				newBlock.SetPrevPtr(currentBlock.GetPrevPtr());
				newBlock.SetNextPtr(currentBlock.GetNextPtr());

				m_Head = newBlock;

				return currentBlock.GetPayloadPtr();
			}
			else
			{
				currentBlock = currentBlock.GetNextPtr();
			}
		}

		AR_CORE_ASSERT(false, "No Room!");
		return nullptr;
	}

	void* MemoryPool::Allocate(uint32_t size, uint32_t alignment, uint32_t alignmentOffset)
	{
		AR_CORE_TRACE("ALLOCATING: Size - {:10} Alignment - {:10} Alignment Offset: {:10}", size, alignment, alignmentOffset);
		AR_CORE_ASSERT(m_Data, "Cannot use default constructed MemoryPool!");

		uint32_t adjustedAlignment = AdjustAlignment(alignment);
		uint32_t adjustedSize = AdjustSize(size, adjustedAlignment, alignmentOffset);

		MemoryBlock currentBlock = m_Head;

		while (currentBlock)
		{
			if (currentBlock.CanFitAndSplit(adjustedSize))
			{
				uint32_t beforeSplitSize = currentBlock.GetBlockSize();
				bool isCurrentLast = currentBlock.IsLastBlock();
				currentBlock.SetPayloadSize(adjustedSize);
				currentBlock.SetAlloc(true);

				MemoryBlock newBlock = reinterpret_cast<char*>(currentBlock.GetFooter()) + 4;
				uint32_t newBlockSize = beforeSplitSize - currentBlock.GetBlockSize();
				newBlock.SetBlockSize(newBlockSize);
				newBlock.SetAlloc(false);

				if (isCurrentLast)
				{
					newBlock.GetFooter()->SetSize(0);
					newBlock.GetFooter()->SetAlloc(true);
				}

				newBlock.SetPrevPtr(currentBlock.GetPrevPtr());
				newBlock.SetNextPtr(currentBlock.GetNextPtr());

				m_Head = newBlock;

				return currentBlock.GetPayloadPtr(adjustedAlignment, alignmentOffset);
			}
			else
			{
				currentBlock = currentBlock.GetNextPtr();
			}
		}

		AR_CORE_ASSERT(false, "No Room!");
		return nullptr;
	}

	void MemoryPool::Deallocate(void* ptr)
	{
		if (!ptr)
			return;

		uintptr_t currentPtr = reinterpret_cast<uintptr_t>(ptr) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

		BlockData* tempHeader = reinterpret_cast<BlockData*>(currentPtr);
		BlockData* tempFooter = tempHeader->GetSize() <= m_Size ? reinterpret_cast<BlockData*>(currentPtr + static_cast<uintptr_t>(tempHeader->GetSize()) - 4) : nullptr;
		while (tempHeader->GetSize() == 0 || !tempFooter || *tempHeader != *tempFooter)
		{
			currentPtr -= AR_PLATFORM_MIN_MALLOC_ALIGNMENT;
			tempHeader = reinterpret_cast<BlockData*>(currentPtr);
			tempFooter = tempHeader->GetSize() <= m_Size ? reinterpret_cast<BlockData*>(currentPtr + static_cast<uintptr_t>(tempHeader->GetSize()) - 4) : nullptr;
		}

		MemoryBlock freedBlock = currentPtr;
		MemoryBlock currentBlock = m_Head;
		while (currentBlock != nullptr && currentBlock < freedBlock)
		{
			currentBlock = currentBlock.GetNextPtr();
		}

		if (currentBlock == nullptr)
		{
			freedBlock.SetAlloc(false);
			freedBlock.SetPrevPtr(nullptr);
			freedBlock.SetNextPtr(nullptr);

			if (m_Head != nullptr)
			{
				MemoryBlock head = m_Head;
				while (head.GetNextPtr() != nullptr)
				{
					head = head.GetNextPtr();
				}
				head.SetNextPtr(freedBlock);
			}
			else
			{
				m_Head = freedBlock;
			}
		}
		else
		{
			freedBlock.SetAlloc(false);
			freedBlock.SetPrevPtr(currentBlock.GetPrevPtr());
			freedBlock.SetNextPtr(currentBlock);

			if (currentBlock.GetPrevPtr() != nullptr)
			{
				currentBlock.GetPrevPtr().SetNextPtr(freedBlock);
			}
			else
			{
				m_Head = freedBlock;
			}

			currentBlock.SetPrevPtr(freedBlock);
		}

		if (freedBlock != m_PoolStart)
		{
			BlockData* leftBlockFooter = reinterpret_cast<BlockData*>(freedBlock - 4);
			if (!leftBlockFooter->GetAlloc())
			{
				MemoryBlock leftBlock = freedBlock - leftBlockFooter->GetSize();
				leftBlock.SetNextPtr(freedBlock.GetNextPtr());
				if (leftBlock.GetNextPtr() != nullptr) leftBlock.GetNextPtr().SetPrevPtr(leftBlock);
				leftBlock.SetBlockSize(leftBlock.GetBlockSize() + freedBlock.GetBlockSize());
				freedBlock = leftBlock;
			#if AR_BUILD_DEBUG
				std::memset(reinterpret_cast<void*>(freedBlock + 4 + AR_PLATFORM_PTR_SIZE * 2), 0, freedBlock.GetPayloadSize() - AR_PLATFORM_PTR_SIZE * 2);
			#endif
			}
		}

		if (!freedBlock.IsLastBlock())
		{
			MemoryBlock rightBlock = freedBlock + freedBlock.GetBlockSize();
			bool isLast = rightBlock.IsLastBlock();
			if (!rightBlock.GetAlloc())
			{
				freedBlock.SetNextPtr(rightBlock.GetNextPtr());
				if (freedBlock.GetNextPtr() != nullptr) freedBlock.GetNextPtr().SetPrevPtr(freedBlock);
				freedBlock.SetBlockSize(freedBlock.GetBlockSize() + rightBlock.GetBlockSize());
				if (isLast)
				{
					freedBlock.GetFooter()->SetSize(0);
					freedBlock.GetFooter()->SetAlloc(true);
				}
			#if AR_BUILD_DEBUG
				std::memset(reinterpret_cast<void*>(freedBlock + 4 + AR_PLATFORM_PTR_SIZE * 2), 0, freedBlock.GetPayloadSize() - AR_PLATFORM_PTR_SIZE * 2);
			#endif
			}
		}
 	}

	uint32_t MemoryPool::AdjustSize(const uint32_t& size)
	{
		return (size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	uint32_t MemoryPool::AdjustSize(const uint32_t& size, const uint32_t& alignment, const uint32_t& offset)
	{
		return (size + alignment + offset + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	uint32_t MemoryPool::AdjustAlignment(const uint32_t& alignment)
	{
		uint32_t adjustedAlignment = alignment >= AR_PLATFORM_MIN_MALLOC_ALIGNMENT ? alignment : AR_PLATFORM_MIN_MALLOC_ALIGNMENT;

		if ((adjustedAlignment & (adjustedAlignment - 1)) != 0)
		{
			AR_CORE_ASSERT(false, "Alignment must be a power of 2!");
			throw std::invalid_argument("Alignment must be a power of 2!");
		}

		return adjustedAlignment;
	}

	void* AppAllocator::allocate(size_t size, int flags) const
	{
		AR_CORE_ASSERT(m_Manager, "Tried to use allocator without MemoryManager!");
		return m_Manager->Allocate(size);
	}

	void* AppAllocator::allocate(size_t size, size_t alignment, size_t offset, int flags) const
	{
		AR_CORE_ASSERT(m_Manager, "Tried to use allocator without MemoryManager!");
		return m_Manager->Allocate(size, alignment, offset);
	}

	void AppAllocator::deallocate(void* ptr, size_t size) const
	{
		AR_CORE_ASSERT(m_Manager, "Tried to use allocator without MemoryManager!");
		m_Manager->Deallocate(ptr, size);
	}

	AppAllocator* get_default_allocator(const AppAllocator* alloc) { return ::Ares::Application::Get().GetMemoryManager().GetDefaultAllocator(); }

}