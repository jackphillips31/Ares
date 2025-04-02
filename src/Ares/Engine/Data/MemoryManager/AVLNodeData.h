#pragma once

namespace Ares::Internal {

	/*
	class MemoryBlockNew;

	struct AVLNodeData
	{
		AVLNodeData* LeftPtr;
		AVLNodeData* RightPtr;
		AVLNodeData* ParentPtr;
		int32_t Height;

		void Init();
		size_t GetBlockSize();
		//MemoryBlockNew GetMemoryBlock();

		operator MemoryBlockNew();
	};
	*/

	struct FreeBlock;

	struct AVLNode
	{
		AVLNode* LeftPtr;
		AVLNode* RightPtr;
		AVLNode* ParentPtr;
		AVLNode* FreeListNext;
		uint32_t Height;

		FreeBlock* GetFreeBlock() const;
	};

}