#include <arespch.h>
#include "Engine/Data/MemoryManager/BlockData.h"

namespace Ares::Internal {

	uint32_t BlockData::GetSize() const
	{
		return m_Data & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	bool BlockData::GetAlloc() const
	{
		return (m_Data & 1) != 0;
	}

	void BlockData::SetSize(const uint32_t size)
	{
		m_Data = size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	void BlockData::SetAlloc(const bool isAlloc)
	{
		if (isAlloc)
			m_Data |= 1;
		else
			m_Data &= ~1;
	}

	uint32_t Metadata::GetSize() const
	{
		return m_Data & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	bool Metadata::GetAlloc() const
	{
		return (m_Data & 1) != 0;
	}

	void Metadata::SetSize(const uint32_t size)
	{
#if AR_BUILD_DEBUG
		if (size % AR_PLATFORM_MIN_MALLOC_ALIGNMENT != 0)
		{
			AR_CORE_ASSERT(false, "Invalid block size!");
			throw std::invalid_argument("Invalid block size!");
		}

		m_Data = (size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1)) | (m_Data & AR_PLATFORM_MIN_MALLOC_ALIGNMENT);
#else
		m_Data = size | (m_Data & AR_PLATFORM_MIN_MALLOC_ALIGNMENT);
#endif
	}

	void Metadata::SetAlloc(const bool isAlloc)
	{
		m_Data = (m_Data & ~1) | static_cast<uint32_t>(isAlloc);
	}

}