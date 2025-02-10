#pragma once

namespace Ares::Internal {

	struct BlockData;
	struct AVLNodeData;

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

}