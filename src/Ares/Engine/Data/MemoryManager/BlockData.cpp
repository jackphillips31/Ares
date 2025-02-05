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

	void BlockData::SetSize(const uint32_t& size)
	{
		m_Data = size & ~(AR_PLATFORM_MIN_MALLOC_ALIGNMENT - 1);
	}

	void BlockData::SetAlloc(const bool& isAlloc)
	{
		if (isAlloc)
			m_Data |= 1;
		else
			m_Data &= ~1;
	}

}