#include <arespch.h>

#include "Engine/Data/FileBuffer.h"

namespace Ares {

	FileBuffer::FileBuffer(const RawData& data)
		: m_Size(data.Size)
	{
		if (!data.Data && data.Size)
		{
			m_Data = CreateScope<uint8_t[]>(data.Size);
		}
		else if (!data)
		{
			m_Data = nullptr;
		}
		else
		{
			m_Data = CreateScope<uint8_t[]>(data.Size);

			std::memcpy(m_Data.get(), data.Data, data.Size);
		}
	}

	FileBuffer::FileBuffer(FileBuffer&& other) noexcept
		: m_Size(other.m_Size)
	{
		if (!other.m_Data)
		{
			m_Data = nullptr;
		}
		else
		{
			m_Data = std::move(other.m_Data);
		}
	}

	FileBuffer::~FileBuffer()
	{
		m_Data.reset();
	}

}