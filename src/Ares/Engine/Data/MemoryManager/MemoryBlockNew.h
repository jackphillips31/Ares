#pragma once
#include "Engine/Data/MemoryManager/BlockData.h"

namespace Ares::Internal {

	struct AVLNodeData;

	/*
	class MemoryBlockNew
	{
	public:
		MemoryBlockNew();
		MemoryBlockNew(void* block);
		MemoryBlockNew(void* block, const size_t& size);
		~MemoryBlockNew();

		bool IsValid() const;
		bool IsLastBlock() const;

		size_t GetSize() const;
		bool GetAlloc() const;
		void* GetPayloadPtr(const size_t& adjustedAlignment, const size_t& offset) const;
		MemoryBlockNew GetAVLNodeLeft() const;
		MemoryBlockNew GetAVLNodeRight() const;
		MemoryBlockNew GetAVLNodeParent() const;
		uint32_t GetAVLNodeHeight() const;
		AVLNodeData* GetAVLNode() const;

		void SetSize(size_t blockSize);
		void SetAlloc(bool isAlloc);
		void SetLastBlock();
		void SetAVLNodeLeft(const MemoryBlockNew& leftBlock);
		void SetAVLNodeLeft(AVLNodeData* node);
		void SetAVLNodeRight(const MemoryBlockNew& rightBlock);
		void SetAVLNodeRight(AVLNodeData* node);
		void SetAVLNodeParent(const MemoryBlockNew& parentBlock);
		void SetAVLNodeParent(AVLNodeData* node);
		void SetAVLNodeHeight(uint32_t height);
		void SetAVLNode(void* leftPtr, void* rightPtr, void* parentPtr, uint32_t height);

		explicit operator bool() const;
		operator char* () const;

	private:
		BlockData* m_Header;
		BlockData* m_Footer;
		AVLNodeData* m_AVLNode;
	};

	*/

	struct AVLNode;

	struct FreeBlock
	{
	public:
		FreeBlock() = delete;
		~FreeBlock() = delete;

		inline uint32_t GetSize() const { return GetHeader()->GetSize(); }
		inline bool GetAlloc() const { return GetHeader()->GetAlloc(); }
		inline bool GetIsLast() const { return (GetFooter()->GetSize() == 0 && GetFooter()->GetAlloc() == true) ? true : false; }
		inline bool GetIsValid() const { if (*GetHeader() == *GetFooter()) return true; else if (GetIsLast()) return true; else return false; }
		AVLNode* GetAVLNode() const;

		inline void SetSize(const size_t size) { SetSize(static_cast<uint32_t>(size)); }
		inline void SetSize(const uint32_t size) { GetHeader()->SetSize(size); GetFooter()->SetSize(size); }
		inline void SetAlloc(const bool isAlloc) { GetHeader()->SetAlloc(isAlloc); GetFooter()->SetAlloc(isAlloc); }
		inline void SetLastBlock(const bool isLast) { if (isLast) { GetFooter()->SetSize(0); GetFooter()->SetAlloc(true); } }

		inline static FreeBlock* Create(void* ptr) { return reinterpret_cast<FreeBlock*>(ptr); }

	private:
		inline BlockData* GetHeader() const { return reinterpret_cast<BlockData*>(const_cast<FreeBlock*>(this)); }
		inline BlockData* GetFooter() const { return reinterpret_cast<BlockData*>(reinterpret_cast<char*>(const_cast<FreeBlock*>(this)) + GetHeader()->GetSize() - sizeof(BlockData)); }
	};

}