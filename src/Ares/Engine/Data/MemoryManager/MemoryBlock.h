#pragma once

namespace Ares::Internal {

	struct BlockData;

	struct MemoryBlock
	{
	public:
		MemoryBlock(void* ptr);
		MemoryBlock(char* ptr);
		MemoryBlock(const uintptr_t& ptr);

		inline void* GetBlock() const { return m_Header; }
		inline BlockData* GetHeader() const { return m_Header; }
		inline BlockData* GetFooter() const { return m_Footer; }
		uint32_t GetBlockSize() const;
		uint32_t GetBlockSizeBasedOnPayload(const uint32_t& adjustedSize) const;
		uint32_t GetPayloadSize() const;
		void* GetPayloadPtr() const;
		void* GetPayloadPtr(const uint32_t& adjustedAlignment, const uint32_t& offset) const;
		bool GetAlloc() const;
		MemoryBlock GetPrevPtr() const;
		MemoryBlock GetNextPtr() const;

		bool IsLastBlock() const;
		bool CanFitAndSplit(const uint32_t& adjustedSize) const;

		void SetBlockSize(const uint32_t& size);
		void SetPayloadSize(const uint32_t& adjustedSize);
		void SetAlloc(const bool& isAlloc);
		void SetPrevPtr(void* ptr);
		void SetPrevPtr(const MemoryBlock& block);
		void SetNextPtr(void* ptr);
		void SetNextPtr(const MemoryBlock& block);

		inline operator void* () const { return m_Header; }
		inline operator uintptr_t() const { return reinterpret_cast<uintptr_t>(m_Header); }
		inline bool operator<(const MemoryBlock& other) const { return m_Header < other.m_Header; }
		inline bool operator>(const MemoryBlock& other) const { return m_Header > other.m_Header; }

	private:
		inline void CheckFooter();
		inline uint32_t AdjustAlignment(const uint32_t& alignment) const;

	private:
		BlockData* m_Header;
		BlockData* m_Footer;
	};

}