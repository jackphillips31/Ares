#pragma once

namespace Ares::Internal {

	class MemoryPool
	{
	public:
		MemoryPool(const size_t& size);
		~MemoryPool();

		MemoryPool(const MemoryPool&) = delete;
		MemoryPool& operator=(const MemoryPool&) = delete;

		MemoryPool(MemoryPool&& other) noexcept;
		MemoryPool& operator=(MemoryPool&& other) noexcept;

		void* Allocate(const uint32_t& size);
		void* Allocate(const uint32_t& size, const uint32_t& alignment, const uint32_t& alignmentOffset);

		void Deallocate(void* ptr);

	private:
		uint32_t AdjustSize(const uint32_t& size) const;
		uint32_t AdjustSize(const uint32_t& size, const uint32_t& alignment, const uint32_t& offset) const;
		uint32_t AdjustAlignment(const uint32_t& alignment) const;

	private:
		void* m_Data;
		void* m_Head;
		uint32_t m_Size;
		uintptr_t m_PoolStart;
	};

}