#include <arespch.h>
#include "Engine/Data/MemoryManager/AVLNodeData.h"

#include "Engine/Data/MemoryManager/BlockData.h"
#include "Engine/Data/MemoryManager/MemoryBlockNew.h"

namespace Ares::Internal {

	constexpr size_t g_MetaDataSize = sizeof(BlockData);
	constexpr size_t g_FreeListDataSize = sizeof(AVLNode);
	constexpr size_t g_PayloadPaddingFront = AR_PLATFORM_MIN_MALLOC_ALIGNMENT - g_MetaDataSize;
	constexpr size_t g_MinimumBlockSize = (g_MetaDataSize + g_PayloadPaddingFront + g_FreeListDataSize + g_MetaDataSize + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

	FreeBlock* AVLNode::GetFreeBlock() const
	{
		return reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(const_cast<AVLNode*>(this)) - g_PayloadPaddingFront - g_MetaDataSize);
	}

	/*
	void AVLNodeData::Init()
	{
		LeftPtr = nullptr;
		RightPtr = nullptr;
		ParentPtr = nullptr;
		Height = 0;
	}

	size_t AVLNodeData::GetBlockSize()
	{
		return static_cast<size_t>(reinterpret_cast<BlockData*>(reinterpret_cast<char*>(this) - g_MetaDataSize - g_PayloadPaddingFront)->GetSize());
	}

	MemoryBlockNew AVLNodeData::GetMemoryBlock()
	{
		return static_cast<MemoryBlockNew>(*this);
	}

	AVLNodeData::operator MemoryBlockNew()
	{
		void* block = reinterpret_cast<void*>(reinterpret_cast<char*>(this) - g_PayloadPaddingFront - g_MetaDataSize);

		if (block == nullptr)
		{
			AR_CORE_ASSERT(false, "MemoryBlock pointer is invalid!");
			throw std::runtime_error("MemoryBlock pointer is invalid!");
		}

		return MemoryBlockNew(block);
	}
	*/

}