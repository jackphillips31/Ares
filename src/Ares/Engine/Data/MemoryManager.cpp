#include <arespch.h>
#include "Engine/Data/MemoryManager.h"

namespace Ares::Internal {

	MemoryManager::MemoryManager(size_t size)
	{
		size_t totalSize = size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT * 2;
		m_Size = static_cast<uint32_t>(totalSize);

		m_Data = malloc(totalSize);
		if (m_Data)
			std::memset(m_Data, 0, totalSize);

		uintptr_t dataStart = reinterpret_cast<uintptr_t>(m_Data);
		m_BlockStart =
			dataStart % AR_PLATFORM_MIN_MALLOC_ALIGNMENT == 0 ?
			dataStart :
			(dataStart + AR_PLATFORM_MIN_MALLOC_ALIGNMENT) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		uintptr_t blockEnd = (dataStart + totalSize) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
		uint32_t blockSize = static_cast<uint32_t>(blockEnd - m_BlockStart);

		MemoryBlock mainBlock = m_BlockStart;
		mainBlock.SetBlockSize(blockSize);
		mainBlock.SetAlloc(false);
		mainBlock.GetFooter()->SetSize(0);
		mainBlock.GetFooter()->SetAlloc(true);
		mainBlock.SetPrevPtr(nullptr);
		mainBlock.SetNextPtr(nullptr);

		m_Head = mainBlock;
	}

	MemoryManager::~MemoryManager()
	{
		free(m_Data);
	}

	void* MemoryManager::Allocate(uint32_t size)
	{
		uint32_t adjustedSize =
			(size % AR_PLATFORM_MIN_MALLOC_ALIGNMENT == 0) ?
			size :
			(size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

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

	void* MemoryManager::Allocate(uint32_t size, uint32_t alignment, uint32_t alignmentOffset)
	{
		return nullptr;
	}

	void MemoryManager::Deallocate(void* ptr)
	{
		MemoryBlock freedBlock = static_cast<char*>(ptr) - 4;
		void* current = m_Head;

		while (current != nullptr && reinterpret_cast<uintptr_t>(current) < freedBlock)
		{
			current = MemoryBlock(current).GetNextPtr();
		}

		if (current == nullptr)
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
			MemoryBlock currentBlock = current;
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

		// Check if left block is free
		/**
		 * set left next = current next
		 * set left next->prev = left
		 * set left size = left size + current size
		 */

		// Check if right block is free
		/**
		 * set current next = right next
		 * if current next != nullptr then current next->prev = current
		 * set current size = current size + right size
		 */

		// coalesce
		/**
		 * From freed block - check if left is free & join
		 * From next block - check if left is free & join
		 */
	}

}