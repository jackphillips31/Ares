#include <arespch.h>

#include "Engine/Data/FileBuffer.h"

namespace Ares {

	FileBuffer::FileBuffer(const void* data, size_t size)
		: m_Size(size)
	{
		if (!data && size)
		{
			m_Data = CreateScope<uint8_t[]>(size);
		}
		else if (!data && !size)
		{
			m_Data = nullptr;
		}
		else
		{
			m_Data = CreateScope<uint8_t[]>(size);

			std::memcpy(m_Data.get(), data, size);
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