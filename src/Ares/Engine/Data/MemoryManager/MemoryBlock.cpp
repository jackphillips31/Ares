#include <arespch.h>
#include "Engine/Data/MemoryManager/MemoryBlock.h"

#include "Engine/Data/MemoryManager/BlockData.h"

namespace Ares::Internal {

	MemoryBlock::MemoryBlock(void* ptr)
		: m_Header(static_cast<BlockData*>(ptr)), m_Footer(nullptr)
	{
		CheckFooter();
	}

	MemoryBlock::MemoryBlock(char* ptr)
		: m_Header(reinterpret_cast<BlockData*>(ptr)), m_Footer(nullptr)
	{
		CheckFooter();
	}

	MemoryBlock::MemoryBlock(const uintptr_t& ptr)
		: m_Header(reinterpret_cast<BlockData*>(ptr)), m_Footer(nullptr)
	{
		CheckFooter();
	}

	uint32_t MemoryBlock::GetBlockSize() const
	{
		return m_Header->GetSize();
	}

	uint32_t MemoryBlock::GetBlockSizeBasedOnPayload(const uint32_t& adjustedSize) const
	{
		return (adjustedSize + 8 + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	uint32_t MemoryBlock::GetPayloadSize() const
	{
		return m_Header->GetSize() - 8;
	}

	void* MemoryBlock::GetPayloadPtr() const
	{
		/*
		uintptr_t header = reinterpret_cast<uintptr_t>(m_Header);
		uintptr_t padding = (header + 4) % AR_PLATFORM_MIN_MALLOC_ALIGNMENT;
		uintptr_t userPtr = (header + 4 + padding + 8 - 1) & ~(8 - 1);
		return reinterpret_cast<void*>(userPtr);
		*/

		/**
		 * +4 for block header
		 *
		 * Using 8 as minimum alignment
		 * Subject to change
		 */
		return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(m_Header) + 4 + 8 - 1) & ~(8 - 1));
	}

	void* MemoryBlock::GetPayloadPtr(const uint32_t& adjustedAlignment, const uint32_t& offset) const
	{
		/*
		uintptr_t header = reinterpret_cast<uintptr_t>(m_Header);
		uintptr_t padding = (header + 4) % adjustedAlignment;
		uintptr_t userPtr = header + 4 + padding + offset;
		return reinterpret_cast<void*>(userPtr);
		*/

		/**
		 * +4 for block header
		 */
		return reinterpret_cast<void*>(
			((reinterpret_cast<uintptr_t>(m_Header) + 4 + static_cast<uintptr_t>(adjustedAlignment) - 1) & ~(static_cast<uintptr_t>(adjustedAlignment) - 1)) + static_cast<uintptr_t>(offset)
		);
	}

	bool MemoryBlock::GetAlloc() const
	{
		return m_Header->GetAlloc();
	}

	MemoryBlock MemoryBlock::GetPrevPtr() const
	{
		return *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4);
	}

	MemoryBlock MemoryBlock::GetNextPtr() const
	{
		return *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4 + AR_PLATFORM_PTR_SIZE);
	}

	bool MemoryBlock::IsLastBlock() const
	{
		return m_Footer->GetSize() == 0 && m_Footer->GetAlloc() == true;
	}

	bool MemoryBlock::CanFitAndSplit(const uint32_t& adjustedSize) const
	{
		/**
		 * +8 for allocated block's new footer and split block's new header
		 * 2 pointers for split block's free list
		 */
		if (GetPayloadSize() >= GetBlockSizeBasedOnPayload(adjustedSize) + (AR_PLATFORM_PTR_SIZE * 2) && !GetAlloc())
		{
			return true;
		}

		return false;
	}

	void MemoryBlock::SetBlockSize(const uint32_t& size)
	{
		if (size % AR_PLATFORM_MIN_MALLOC_ALIGNMENT != 0)
		{
			AR_CORE_ASSERT(false, "Invalid Memory Block Size!");
			throw std::invalid_argument("Invalid Memory Block Size!");
		}

		m_Header->SetSize(size);
		m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + size - 4);
		m_Footer->SetSize(size);
	}

	void MemoryBlock::SetPayloadSize(const uint32_t& adjustedSize)
	{
		/**
		 * +8 for block header and footer
		 */
		SetBlockSize(GetBlockSizeBasedOnPayload(adjustedSize));
	}

	void MemoryBlock::SetAlloc(const bool& isAlloc)
	{
		m_Header->SetAlloc(isAlloc);
		if (m_Footer) m_Footer->SetAlloc(isAlloc);
	}

	void MemoryBlock::SetPrevPtr(void* ptr)
	{
		*reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4) = ptr;
	}

	void MemoryBlock::SetPrevPtr(const MemoryBlock& block)
	{
		SetPrevPtr(block.GetBlock());
	}

	void MemoryBlock::SetNextPtr(void* ptr)
	{
		*reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4 + AR_PLATFORM_PTR_SIZE) = ptr;
	}

	void MemoryBlock::SetNextPtr(const MemoryBlock& block)
	{
		SetNextPtr(block.GetBlock());
	}

	void MemoryBlock::CheckFooter()
	{
		if (m_Header != nullptr && m_Header->GetSize() != 0)
			m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + m_Header->GetSize() - 4);
	}

	uint32_t MemoryBlock::AdjustAlignment(const uint32_t& alignment) const
	{
		return alignment >= AR_PLATFORM_MIN_MALLOC_ALIGNMENT ? alignment : AR_PLATFORM_MIN_MALLOC_ALIGNMENT;
	}

}