#pragma once

namespace Ares::Internal {

	class MemoryBlockNew;

	struct AVLNodeData
	{
		AVLNodeData* LeftPtr;
		AVLNodeData* RightPtr;
		AVLNodeData* ParentPtr;
		int32_t Height;

		void Init();
		size_t GetBlockSize();
		MemoryBlockNew GetMemoryBlock();

		operator MemoryBlockNew();
	};

}