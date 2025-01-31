#pragma once
#include "Engine/Core/Core.h"

#define AR_PLATFORM_MIN_MALLOC_ALIGNMENT 8

namespace Ares::Internal {

	class MemoryManager
	{
	private:
		struct MemoryBlock
		{
		private:
			struct BlockData
			{
			public:
				inline uint32_t GetSize() { return m_Data & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1); }
				inline bool GetAlloc() { return (m_Data & 1) != 0; }

				inline void SetSize(const uint32_t& size) { m_Data = size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1); }
				inline void SetAlloc(const bool& isAlloc) { if (isAlloc) m_Data |= 1; else m_Data &= ~1; }

			private:
				uint32_t m_Data;
			};

		public:
			MemoryBlock(void* ptr) : m_Header(static_cast<BlockData*>(ptr)), m_Footer(nullptr) { CheckFooter(); }
			MemoryBlock(char* ptr) : m_Header(reinterpret_cast<BlockData*>(ptr)), m_Footer(nullptr) { CheckFooter(); }
			MemoryBlock(uintptr_t ptr) : m_Header(reinterpret_cast<BlockData*>(ptr)), m_Footer(nullptr) { CheckFooter(); }

			inline void* GetBlock() const { return m_Header; }
			inline BlockData* GetHeader() const { return m_Header; }
			inline BlockData* GetFooter() const { return m_Footer; }
			inline uint32_t GetBlockSize() const { return m_Header->GetSize(); }
			inline uint32_t GetPayloadSize() const { return m_Header->GetSize() - 8; }
			inline void* GetPayloadPtr() const { return reinterpret_cast<char*>(m_Header) + 4; }
			inline bool GetAlloc() const { return m_Header->GetAlloc(); }
			inline MemoryBlock GetPrevPtr() const { return *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4); }
			inline MemoryBlock GetNextPtr() const { return *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4 + AR_PLATFORM_PTR_SIZE); }

			inline bool IsLastBlock() const { return m_Footer->GetSize() == 0 && m_Footer->GetAlloc() == true; }
			inline bool CanFitAndSplit(const uint32_t& size) const
			{
				uint32_t adjustedSize =
					(size % AR_PLATFORM_MIN_MALLOC_ALIGNMENT == 0) ?
					size :
					(size + AR_PLATFORM_MIN_MALLOC_ALIGNMENT) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);

				if ((GetPayloadSize() - adjustedSize) >= 4 + 4 + (AR_PLATFORM_PTR_SIZE * 2) && !GetAlloc())
				{
					return true;
				}
				return false;
			}

			inline void SetBlockSize(const uint32_t& size) 
			{
				m_Header->SetSize(size);
				m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + (size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1)) - 4);
				m_Footer->SetSize(size);
			}
			inline void SetPayloadSize(const uint32_t& size)
			{
				uint32_t adjustedSize = size + 8;
				SetBlockSize(
					(adjustedSize % AR_PLATFORM_MIN_MALLOC_ALIGNMENT == 0) ?
					adjustedSize :
					(adjustedSize + AR_PLATFORM_MIN_MALLOC_ALIGNMENT) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1)
				);
			}
			inline void SetAlloc(const bool& isAlloc)
			{
				m_Header->SetAlloc(isAlloc);
				if (m_Footer)
					m_Footer->SetAlloc(isAlloc);
			}
			inline void SetPrevPtr(void* ptr) { *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4) = ptr; }
			inline void SetPrevPtr(const MemoryBlock& block) { SetPrevPtr(block.GetBlock()); }
			inline void SetNextPtr(void* ptr) { *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4 + AR_PLATFORM_PTR_SIZE) = ptr; }
			inline void SetNextPtr(const MemoryBlock& block) { SetNextPtr(block.GetBlock()); }

			inline operator void*() { return m_Header; }
			inline operator uintptr_t() { return reinterpret_cast<uintptr_t>(m_Header); }

		private:
			inline void CheckFooter()
			{
				if (m_Header != nullptr && m_Header->GetSize() != 0)
					m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + m_Header->GetSize() - 4);
			}

		private:
			BlockData* m_Header;
			BlockData* m_Footer;
		};

	public:
		MemoryManager(size_t size);
		~MemoryManager();

		void* Allocate(uint32_t size);
		void* Allocate(uint32_t size, uint32_t alignment, uint32_t alignmentOffset);

		void Deallocate(void* ptr);

	private:
		void* m_Data;
		void* m_Head;
		uint32_t m_Size;
		uintptr_t m_BlockStart;
	};

}