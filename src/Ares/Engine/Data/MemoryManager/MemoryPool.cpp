#include <arespch.h>
#include "Engine/Data/MemoryManager/MemoryPool.h"

#include "Engine/Data/MemoryManager/AVLNodeData.h"
#include "Engine/Data/MemoryManager/BlockData.h"
#include "Engine/Data/MemoryManager/MemoryBlock.h"
#include "Engine/Data/MemoryManager/MemoryBlockNew.h"

namespace Ares::Internal {

	MemoryPool::MemoryPool(const size_t& size)
	{
		size_t totalSize = (size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		m_Size = static_cast<uint32_t>(totalSize);

		m_Data = malloc(totalSize);
		if (m_Data)
			std::memset(m_Data, 0, totalSize);
		else
			throw std::bad_alloc();

		uintptr_t dataStart = reinterpret_cast<uintptr_t>(m_Data);
		m_PoolStart = (dataStart + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		uintptr_t poolEnd = (dataStart + totalSize) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		uint32_t poolSize = static_cast<uint32_t>(poolEnd - m_PoolStart);

		MemoryBlock mainBlock = m_PoolStart;
		mainBlock.SetBlockSize(poolSize);
		mainBlock.SetAlloc(false);
		mainBlock.GetFooter()->SetSize(0);
		mainBlock.GetFooter()->SetAlloc(true);
		mainBlock.SetPrevPtr(nullptr);
		mainBlock.SetNextPtr(nullptr);

		m_Head = mainBlock;
	}

	MemoryPool::~MemoryPool()
	{
		if (m_Data)
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
		std::shared_lock lock1(other.m_PoolMutex);
		std::unique_lock lock2(m_PoolMutex);
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

	void* MemoryPool::Allocate(const uint32_t& size)
	{
		std::unique_lock lock(m_PoolMutex);
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
				newBlock.SetBlockSize(beforeSplitSize - currentBlock.GetBlockSize());
				newBlock.SetAlloc(false);

				if (isCurrentLast)
				{
					newBlock.GetFooter()->SetSize(0);
					newBlock.GetFooter()->SetAlloc(true);
				}

				newBlock.SetPrevPtr(currentBlock.GetPrevPtr());
				newBlock.SetNextPtr(currentBlock.GetNextPtr());

				if (newBlock.GetPrevPtr() != nullptr)
				{
					newBlock.GetPrevPtr().SetNextPtr(newBlock);
				}

				if (newBlock.GetNextPtr() != nullptr)
				{
					newBlock.GetNextPtr().SetPrevPtr(newBlock);
				}

				if (currentBlock == m_Head)
				{
					m_Head = newBlock;
				}

				return currentBlock.GetPayloadPtr();
			}
			else
			{
				currentBlock = currentBlock.GetNextPtr();
			}
		}

		return nullptr;
	}

	void* MemoryPool::Allocate(const uint32_t& size, const uint32_t& alignment, const uint32_t& alignmentOffset)
	{
		std::unique_lock lock(m_PoolMutex);
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

				if (newBlock.GetPrevPtr() != nullptr)
				{
					newBlock.GetPrevPtr().SetNextPtr(newBlock);
				}

				if (newBlock.GetNextPtr() != nullptr)
				{
					newBlock.GetNextPtr().SetPrevPtr(newBlock);
				}

				if (currentBlock == m_Head)
				{
					m_Head = newBlock;
				}

				return currentBlock.GetPayloadPtr(adjustedAlignment, alignmentOffset);
			}
			else
			{
				currentBlock = currentBlock.GetNextPtr();
			}
		}

		return nullptr;
	}

	void MemoryPool::Deallocate(void* ptr)
	{
		if (!ptr)
			return;

		std::unique_lock lock(m_PoolMutex);
		uintptr_t currentPtr = reinterpret_cast<uintptr_t>(ptr) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

		BlockData* tempHeader = reinterpret_cast<BlockData*>(currentPtr);
		BlockData* tempFooter =
			tempHeader->GetSize() <= m_Size ?
			reinterpret_cast<BlockData*>(currentPtr + static_cast<uintptr_t>(tempHeader->GetSize()) - 4) :
			nullptr;

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
			else if (currentBlock == m_Head)
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

	uint32_t MemoryPool::AdjustSize(const uint32_t& size) const
	{
		return (size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	uint32_t MemoryPool::AdjustSize(const uint32_t& size, const uint32_t& alignment, const uint32_t& offset) const
	{
		return (size + alignment + offset + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	uint32_t MemoryPool::AdjustAlignment(const uint32_t& alignment) const
	{
		uint32_t adjustedAlignment = alignment >= AR_PLATFORM_MIN_MALLOC_ALIGNMENT ? alignment : AR_PLATFORM_MIN_MALLOC_ALIGNMENT;

	#if AR_BUILD_DEBUG
		if ((adjustedAlignment & (adjustedAlignment - 1)) != 0)
		{
			AR_CORE_ASSERT(false, "Alignment must be a power of 2!");
			throw std::invalid_argument("Alignment must be a power of 2!");
		}
	#endif

		return adjustedAlignment;
	}

	constexpr size_t g_MetaDataSize = sizeof(BlockData);
	constexpr size_t g_FreeListDataSize = sizeof(AVLNodeData);
	constexpr size_t g_PayloadPaddingFront = AR_PLATFORM_MIN_MALLOC_ALIGNMENT - g_MetaDataSize;
	constexpr size_t g_MinimumBlockSize = (g_MetaDataSize + g_PayloadPaddingFront + g_FreeListDataSize + g_MetaDataSize + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

	MemoryPoolNew::MemoryPoolNew(size_t poolSize)
	{
		size_t minSize = g_MinimumBlockSize;
		// Ensure the pool size is aligned
		size_t adjustedSize = (poolSize + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

		if (adjustedSize < g_MinimumBlockSize)
		{
			AR_CORE_ASSERT(false, "MemoryPool size is less than the minimum MemoryBlock size!");
			throw std::invalid_argument("MemoryPool size is less than the minimum MemoryBlock size!");
		}

		// Allocate extra space to ensure alignment
		void* rawMemory = malloc(adjustedSize);
		m_Data = rawMemory;
		m_Size = adjustedSize;

		// Align the pool start
		m_PoolStart = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(rawMemory) + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1));

		// Compute usable pool size after alignment
		m_PoolSize = adjustedSize - (reinterpret_cast<uintptr_t>(m_PoolStart) - reinterpret_cast<uintptr_t>(rawMemory));
		m_PoolSize &= ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

		// Create MemoryBlock
		MemoryBlockNew poolBlock = m_PoolStart;
		poolBlock.SetSize(m_PoolSize);
		poolBlock.SetAlloc(false);
		poolBlock.SetLastBlock();

		// Initialize AVL Tree
		m_AVLTree.Insert(poolBlock);
	}

	MemoryPoolNew::MemoryPoolNew(MemoryPoolNew&& other) noexcept
	{
		{
			std::shared_lock lock(other.m_Mutex);
			m_Data = other.m_Data;
			m_PoolStart = other.m_PoolStart;
			m_Size = other.m_Size;
			m_PoolSize = other.m_PoolSize;
			m_AVLTree = other.m_AVLTree;
		}
		{
			std::unique_lock lock(other.m_Mutex);
			other.m_Data = nullptr;
			other.m_PoolStart = nullptr;
			other.m_Size = 0;
			other.m_PoolSize = 0;
			other.m_AVLTree = AVLTree();
		}
	}

	MemoryPoolNew& MemoryPoolNew::operator=(MemoryPoolNew&& other) noexcept
	{
		std::unique_lock lock1(m_Mutex, std::defer_lock);
		std::unique_lock lock2(other.m_Mutex, std::defer_lock);
		std::lock(lock1, lock2);

		m_Data = other.m_Data;
		m_PoolStart = other.m_PoolStart;
		m_Size = other.m_Size;
		m_PoolSize = other.m_PoolSize;
		m_AVLTree = other.m_AVLTree;

		other.m_Data = nullptr;
		other.m_PoolStart = nullptr;
		other.m_Size = 0;
		other.m_PoolSize = 0;
		other.m_AVLTree = AVLTree();

		return *this;
	}

	MemoryPoolNew::~MemoryPoolNew()
	{
		free(m_Data);
	}

	void* MemoryPoolNew::Allocate(const size_t& size)
	{
		return Allocate(size, AR_PLATFORM_MIN_MALLOC_ALIGNMENT, 0);
	}

	void* MemoryPoolNew::Allocate(const size_t& size, const size_t& alignment, const size_t& offset)
	{
		size_t adjustedAlignment =
			alignment < AR_PLATFORM_MIN_MALLOC_ALIGNMENT ?
			AR_PLATFORM_MIN_MALLOC_ALIGNMENT :
			(alignment + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		size_t frontPadding = adjustedAlignment - g_MetaDataSize;
		size_t blockSize = ((g_MetaDataSize * 2) + adjustedAlignment + offset + size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		
		if (blockSize < g_MinimumBlockSize)
			blockSize = g_MinimumBlockSize;

		MemoryBlockNew bestFit = m_AVLTree.FindBestFit(blockSize);

		if (bestFit.GetSize() - blockSize >= g_MinimumBlockSize)
		{
			// Split block
			std::unique_lock lock(m_Mutex);
			bool isBestFitLast = bestFit.IsLastBlock();
			size_t beforeSplitSize = bestFit.GetSize();

			m_AVLTree.Remove(bestFit);

			bestFit.SetSize(blockSize);
			bestFit.SetAlloc(true);

			MemoryBlockNew newBlock(static_cast<void*>(bestFit + blockSize), beforeSplitSize - blockSize);
			newBlock.SetAlloc(false);

			if (isBestFitLast)
				newBlock.SetLastBlock();

			m_AVLTree.Insert(newBlock);

			return bestFit.GetPayloadPtr(adjustedAlignment, offset);
		}
		else
		{
			// Don't split block
			std::unique_lock lock(m_Mutex);
			m_AVLTree.Remove(bestFit);

			return bestFit.GetPayloadPtr(adjustedAlignment, offset);
		}

		return nullptr;
	}

	void MemoryPoolNew::Deallocate(void* ptr, const size_t& size)
	{
		if (!ptr)
			return;

		uintptr_t currentPtr = (reinterpret_cast<uintptr_t>(ptr) - AR_PLATFORM_MIN_MALLOC_ALIGNMENT) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

		while (!MemoryBlockNew(reinterpret_cast<void*>(currentPtr)).IsValid())
			currentPtr -= AR_PLATFORM_MIN_MALLOC_ALIGNMENT;

		MemoryBlockNew freedBlock(reinterpret_cast<void*>(currentPtr));
		bool isFreedLast = freedBlock.IsLastBlock();
		freedBlock.SetAlloc(false);
		freedBlock.GetAVLNode()->Init();

		if (isFreedLast)
			freedBlock.SetLastBlock();

		//m_AVLTree.Insert(freedBlock);

		/*
		if (freedBlock != m_PoolStart)
		{
			BlockData* leftBlockFooter = reinterpret_cast<BlockData*>(freedBlock - g_MetaDataSize);
			BlockData* rightBlockHeader = reinterpret_cast<BlockData*>(freedBlock + freedBlock.GetSize());

			if ()
		}
		*/
	}

}