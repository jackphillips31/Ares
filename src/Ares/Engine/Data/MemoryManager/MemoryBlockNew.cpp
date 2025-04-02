#include <arespch.h>
#include "Engine/Data/MemoryManager/MemoryBlockNew.h"

#include "Engine/Data/MemoryManager/AVLNodeData.h"
#include "Engine/Data/MemoryManager/BlockData.h"

namespace Ares::Internal {

	constexpr size_t g_MetaDataSize = sizeof(BlockData);
	constexpr size_t g_FreeListDataSize = sizeof(AVLNode);
	constexpr size_t g_PayloadPaddingFront = AR_PLATFORM_MIN_MALLOC_ALIGNMENT - g_MetaDataSize;
	constexpr size_t g_MinimumBlockSize = (g_MetaDataSize + g_PayloadPaddingFront + g_FreeListDataSize + g_MetaDataSize + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

	AVLNode* FreeBlock::GetAVLNode() const
	{
		return reinterpret_cast<AVLNode*>(reinterpret_cast<char*>(const_cast<FreeBlock*>(this)) + g_MetaDataSize + g_PayloadPaddingFront);
	}

	/*
	MemoryBlockNew::MemoryBlockNew()
		: m_Header(nullptr), m_Footer(nullptr), m_AVLNode(nullptr)
	{
	}

	MemoryBlockNew::MemoryBlockNew(void* block)
	{
		m_Header = reinterpret_cast<BlockData*>(block);
		m_Footer = reinterpret_cast<BlockData*>(static_cast<char*>(block) + m_Header->GetSize() - g_MetaDataSize);
		m_AVLNode = reinterpret_cast<AVLNodeData*>(static_cast<char*>(block) + g_MetaDataSize + g_PayloadPaddingFront);
	}

	MemoryBlockNew::MemoryBlockNew(void* block, const size_t& size)
	{
		if (size < g_MinimumBlockSize)
		{
			AR_CORE_ASSERT(false, "MemoryBlock size is less than the minimum!");
			throw std::invalid_argument("MemoryBlock size is less than the minimum!");
		}

		m_Header = reinterpret_cast<BlockData*>(block);
		m_Footer = nullptr;
		m_AVLNode = reinterpret_cast<AVLNodeData*>(static_cast<char*>(block) + g_MetaDataSize + g_PayloadPaddingFront);
		SetSize(size);
	}

	MemoryBlockNew::~MemoryBlockNew()
	{
		m_Header = nullptr;
		m_Footer = nullptr;
		m_AVLNode = nullptr;
	}

	bool MemoryBlockNew::IsValid() const
	{
		if (m_Header != nullptr && m_Footer != nullptr)
			if (IsLastBlock())
				return true;
			else
				return *m_Header == *m_Footer;
		else
			return false;
	}

	bool MemoryBlockNew::IsLastBlock() const
	{
		return m_Footer->GetSize() == 0 && m_Footer->GetAlloc() == true;
	}

	size_t MemoryBlockNew::GetSize() const
	{
		if (IsValid())
			return m_Header->GetSize();
		else
		{
			AR_CORE_ASSERT(false, "MemoryBlock header and footer don't match!");
			throw std::runtime_error("MemoryBlock header and footer don't match!");
		}
	}

	bool MemoryBlockNew::GetAlloc() const
	{
		if (IsValid())
			return m_Header->GetAlloc();
		else
		{
			AR_CORE_ASSERT(false, "MemoryBlock header and footer don't match!");
			throw std::runtime_error("MemoryBlock header and footer don't match!");
		}
	}

	void* MemoryBlockNew::GetPayloadPtr(const size_t& adjustedAlignment, const size_t& offset) const
	{
		if (IsValid())
		{
			uintptr_t payloadStart = reinterpret_cast<uintptr_t>(m_Header) + AR_PLATFORM_MIN_MALLOC_ALIGNMENT;
			return reinterpret_cast<void*>(((payloadStart + adjustedAlignment - 1) & ~(adjustedAlignment - 1)) + offset);
		}
		else
		{
			AR_CORE_ASSERT(false, "MemoryBlock header and footer don't match!");
			throw std::runtime_error("MemoryBlock header and footer don't match!");
		}
	}

	MemoryBlockNew MemoryBlockNew::GetAVLNodeLeft() const
	{
		if (m_AVLNode)
			return MemoryBlockNew(m_AVLNode->LeftPtr);
		else
		{
			AR_CORE_ASSERT(false, "AVLNodeData does not exist for this MemoryBlock!");
			throw std::runtime_error("AVLNodeData does not exist for this MemoryBlock!");
		}
	}

	MemoryBlockNew MemoryBlockNew::GetAVLNodeRight() const
	{
		if (m_AVLNode)
			return MemoryBlockNew(m_AVLNode->RightPtr);
		else
		{
			AR_CORE_ASSERT(false, "AVLNodeData does not exist for this MemoryBlock!");
			throw std::runtime_error("AVLNodeData does not exist for this MemoryBlock!");
		}
	}

	MemoryBlockNew MemoryBlockNew::GetAVLNodeParent() const
	{
		if (m_AVLNode)
			return MemoryBlockNew(m_AVLNode->ParentPtr);
		else
		{
			AR_CORE_ASSERT(false, "AVLNodeData does not exist for this MemoryBlock!");
			throw std::runtime_error("AVLNodeData does not exist for this MemoryBlock!");
		}
	}

	uint32_t MemoryBlockNew::GetAVLNodeHeight() const
	{
		if (m_AVLNode)
			return m_AVLNode->Height;
		else
		{
			AR_CORE_ASSERT(false, "AVLNodeData does not exist for this MemoryBlock!");
			throw std::runtime_error("AVLNodeData does not exist for this MemoryBlock!");
		}
	}

	AVLNodeData* MemoryBlockNew::GetAVLNode() const
	{
		if (m_AVLNode)
			return m_AVLNode;
		else
		{
			AR_CORE_ASSERT(false, "AVLNodeData does not exist for this MemoryBlock!");
			throw std::runtime_error("AVLNodeData does not exist for this MemoryBlock!");
		}
	}

	void MemoryBlockNew::SetSize(size_t blockSize)
	{
		m_Header->SetSize(static_cast<uint32_t>(blockSize));
		m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + m_Header->GetSize() - g_MetaDataSize);
		m_Footer->SetSize(static_cast<uint32_t>(blockSize));
	}

	void MemoryBlockNew::SetAlloc(bool isAlloc)
	{
		if (IsValid())
		{
			m_Header->SetAlloc(isAlloc);
			m_Footer->SetAlloc(isAlloc);
		}
		else
		{
			AR_CORE_ASSERT(false, "MemoryBlock header and footer don't match!");
			throw std::runtime_error("MemoryBlock header and footer don't match!");
		}
	}

	void MemoryBlockNew::SetLastBlock()
	{
		m_Footer->SetSize(0);
		m_Footer->SetAlloc(true);
	}

	void MemoryBlockNew::SetAVLNodeLeft(const MemoryBlockNew& leftBlock)
	{
		SetAVLNodeLeft(leftBlock.GetAVLNode());
	}

	void MemoryBlockNew::SetAVLNodeLeft(AVLNodeData* node)
	{
		if (m_AVLNode)
			m_AVLNode->LeftPtr = node;
	}

	void MemoryBlockNew::SetAVLNodeRight(const MemoryBlockNew& rightBlock)
	{
		SetAVLNodeRight(rightBlock.GetAVLNode());
	}

	void MemoryBlockNew::SetAVLNodeRight(AVLNodeData* node)
	{
		if (m_AVLNode)
			m_AVLNode->RightPtr = node;
	}

	void MemoryBlockNew::SetAVLNodeParent(const MemoryBlockNew& parentBlock)
	{
		SetAVLNodeRight(parentBlock.GetAVLNode());
	}

	void MemoryBlockNew::SetAVLNodeParent(AVLNodeData* node)
	{
		if (m_AVLNode)
			m_AVLNode->ParentPtr = node;
	}

	void MemoryBlockNew::SetAVLNodeHeight(uint32_t height)
	{
		if (m_AVLNode)
			m_AVLNode->Height = height;
	}

	void MemoryBlockNew::SetAVLNode(void* leftPtr, void* rightPtr, void* parentPtr, uint32_t height)
	{
		if (IsValid() && m_AVLNode)
		{
			m_AVLNode->LeftPtr = reinterpret_cast<AVLNodeData*>(leftPtr);
			m_AVLNode->RightPtr = reinterpret_cast<AVLNodeData*>(rightPtr);
			m_AVLNode->ParentPtr = reinterpret_cast<AVLNodeData*>(parentPtr);
			m_AVLNode->Height = 0;
		}
		else
		{
			AR_CORE_ASSERT(false, "MemoryBlock header and footer don't match!");
			throw std::runtime_error("MemoryBlock header and footer don't match!");
		}
	}

	MemoryBlockNew::operator bool() const
	{
		if (!m_Header && !m_Footer && !m_AVLNode)
			return false;
		else
			return true;
	}

	MemoryBlockNew::operator char* () const
	{
		return reinterpret_cast<char*>(m_Header);
	}

	*/

}