#pragma once
#include "Engine/Core/Core.h"

namespace Ares::Internal {

	class MemoryPool
	{
	private:
		struct BlockData
		{
		public:
			inline uint32_t GetSize() { return m_Data & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1); }
			inline bool GetAlloc() { return (m_Data & 1) != 0; }

			inline void SetSize(const uint32_t& size) { m_Data = size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1); }
			inline void SetAlloc(const bool& isAlloc) { if (isAlloc) m_Data |= 1; else m_Data &= ~1; }

			inline bool operator==(const BlockData& other) { return m_Data == other.m_Data; }
			inline bool operator!=(const BlockData& other) { return m_Data != other.m_Data; }

		private:
			uint32_t m_Data;
		};

		struct MemoryBlock
		{
		public:
			MemoryBlock(void* ptr) : m_Header(static_cast<BlockData*>(ptr)), m_Footer(nullptr) { CheckFooter(); }
			MemoryBlock(char* ptr) : m_Header(reinterpret_cast<BlockData*>(ptr)), m_Footer(nullptr) { CheckFooter(); }
			MemoryBlock(uintptr_t ptr) : m_Header(reinterpret_cast<BlockData*>(ptr)), m_Footer(nullptr) { CheckFooter(); }

			inline void* GetBlock() const { return m_Header; }
			inline BlockData* GetHeader() const { return m_Header; }
			inline BlockData* GetFooter() const { return m_Footer; }
			inline uint32_t GetBlockSize() const { return m_Header->GetSize(); }
			inline uint32_t GetBlockSizeBasedOnPayload(const uint32_t& adjustedSize) const { return (adjustedSize + 8 + AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1) & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1); }
			inline uint32_t GetPayloadSize() const { return m_Header->GetSize() - 8; }
			inline void* GetPayloadPtr() const
			{
				/**
				 * +4 for block header
				 * 
				 * Using 8 as minimum alignment
				 * Subject to change
				 */
				return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(m_Header) + 4 + 8 - 1) & ~(8 - 1));
			}
			inline void* GetPayloadPtr(const uint32_t& adjustedAlignment, const uint32_t& offset) const
			{
				/**
				 * +4 for block header
				 */
				return reinterpret_cast<void*>(((reinterpret_cast<uintptr_t>(m_Header) + 4 + static_cast<uintptr_t>(adjustedAlignment) - 1) & ~(static_cast<uintptr_t>(adjustedAlignment) - 1)) + static_cast<uintptr_t>(offset));
			}
			inline bool GetAlloc() const { return m_Header->GetAlloc(); }
			inline MemoryBlock GetPrevPtr() const { return *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4); }
			inline MemoryBlock GetNextPtr() const { return *reinterpret_cast<void**>(reinterpret_cast<char*>(m_Header) + 4 + AR_PLATFORM_PTR_SIZE); }

			inline bool IsLastBlock() const { return m_Footer->GetSize() == 0 && m_Footer->GetAlloc() == true; }
			inline bool CanFitAndSplit(const uint32_t& adjustedSize) const
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

			inline void SetBlockSize(const uint32_t& size) 
			{
				m_Header->SetSize(size);
				m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + (size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1)) - 4);
				m_Footer->SetSize(size);
			}
			inline void SetPayloadSize(const uint32_t& adjustedSize)
			{
				/**
				 * +8 for block header and footer
				 */
				SetBlockSize(GetBlockSizeBasedOnPayload(adjustedSize));
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

			inline operator void*() const { return m_Header; }
			inline operator uintptr_t() const { return reinterpret_cast<uintptr_t>(m_Header); }
			inline bool operator<(const MemoryBlock& other) const { return m_Header < other.m_Header; }
			inline bool operator>(const MemoryBlock& other) const { return m_Header > other.m_Header; }

		private:
			inline void CheckFooter()
			{
				if (m_Header != nullptr && m_Header->GetSize() != 0)
					m_Footer = reinterpret_cast<BlockData*>(reinterpret_cast<char*>(m_Header) + m_Header->GetSize() - 4);
			}
			inline uint32_t AdjustAlignment(const uint32_t& alignment) const
			{
				return alignment >= AR_PLATFORM_MIN_MALLOC_ALIGNMENT ? alignment : AR_PLATFORM_MIN_MALLOC_ALIGNMENT;
			}

		private:
			BlockData* m_Header;
			BlockData* m_Footer;
		};

	public:
		MemoryPool(size_t size);
		~MemoryPool();

		MemoryPool(const MemoryPool&) = delete;
		MemoryPool& operator=(const MemoryPool&) = delete;

		MemoryPool(MemoryPool&& other) noexcept;
		MemoryPool& operator=(MemoryPool&& other) noexcept;

		void* Allocate(uint32_t size);
		void* Allocate(uint32_t size, uint32_t alignment, uint32_t alignmentOffset);

		void Deallocate(void* ptr);

	private:
		uint32_t AdjustSize(const uint32_t& size);
		uint32_t AdjustSize(const uint32_t& size, const uint32_t& alignment, const uint32_t& offset);
		uint32_t AdjustAlignment(const uint32_t& alignment);

	private:
		void* m_Data;
		void* m_Head;
		uint32_t m_Size;
		uintptr_t m_PoolStart;
	};

	class MemoryManager;

	class AppAllocator : public eastl::allocator
	{
	public:
		AppAllocator() : m_Manager(nullptr) {}
		explicit AppAllocator(MemoryManager* manager) : m_Manager(manager) {}
		explicit AppAllocator(MemoryManager& manager) : m_Manager(&manager) {}

		AppAllocator(const AppAllocator& other) : m_Manager(other.m_Manager) {}
		AppAllocator& operator=(const AppAllocator& other) { m_Manager = other.m_Manager; return *this; }

		void* allocate(size_t size, int flags = 0) const;
		void* allocate(size_t size, size_t alignment, size_t offset, int flags = 0) const;
		void deallocate(void* ptr, size_t size = 0) const;

		bool operator==(const AppAllocator& other) { return m_Manager == other.m_Manager; }
		bool operator!=(const AppAllocator& other) { return m_Manager != other.m_Manager; }

		MemoryManager* GetManager() const { return m_Manager; }

	private:
		MemoryManager* m_Manager;
	};

	class MemoryManager
	{
	private:
		class SelfAllocator : public eastl::allocator
		{
		public:
			explicit SelfAllocator(MemoryManager* manager) : m_Manager(manager) {}

			inline void* allocate(size_t size, int flags = 0) { return m_Manager->SelfAllocate(size); }
			inline void* allocate(size_t size, size_t alignment, size_t offset, int flags = 0) { return m_Manager->SelfAllocate(size, alignment, offset); }
			inline void deallocate(void* ptr, size_t size) { m_Manager->SelfDeallocate(ptr, size); }

		private:
			MemoryManager* m_Manager;
		};
	public:
		MemoryManager();
		~MemoryManager();

		void* Allocate(size_t size, int flags = 0);
		void* Allocate(size_t size, size_t alignment, size_t offset, int flags = 0);
		void Deallocate(void* ptr, size_t size = 0);

		inline AppAllocator* GetDefaultAllocator() { return &m_DefaultAllocator; }

	private:
		void* SelfAllocate(size_t size);
		void* SelfAllocate(size_t size, size_t alignment, size_t offset);
		void SelfDeallocate(void* ptr, size_t size);

	private:
		MemoryPool m_SelfPool;
		SelfAllocator m_SelfAllocator;
		AppAllocator m_DefaultAllocator;
		eastl::hash_map<uintptr_t, size_t, eastl::hash<uintptr_t>, eastl::equal_to<uintptr_t>, MemoryManager::SelfAllocator> m_PtrPoolMap;
		eastl::vector<MemoryPool, MemoryManager::SelfAllocator> m_Pools;
	};

	//bool operator==(const AppAllocator& a, const AppAllocator& b) { return a.GetManager() == b.GetManager(); }
	//bool operator!=(const AppAllocator& a, const AppAllocator& b) { return a.GetManager() != b.GetManager(); }

	struct Deleter
	{
	public:
		Deleter() {}
		Deleter(const AppAllocator& alloc) : m_Allocator(alloc) {}
		Deleter(const Deleter& other) : m_Allocator(other.m_Allocator) {}
		Deleter& operator=(const Deleter& other) { m_Allocator = other.m_Allocator; return *this; }

		void operator()(void* ptr)
		{
			m_Allocator.deallocate(ptr);
		}

	private:
		AppAllocator m_Allocator;
	};

	AppAllocator* get_default_allocator(const AppAllocator* alloc);

}