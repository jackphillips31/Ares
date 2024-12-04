#include <arespch.h>

#include "Engine/Data/FileBuffer.h"

namespace Ares {

	FileBuffer::FileBuffer(const void* data, size_t size)
		: m_Size(size)
	{
		if (!data && size != 0)
		{
			m_Data = CreateScope<uint8_t[]>(size);
		}
		else if (!data && size == 0)
		{
			m_Data = nullptr;
		}
		else
		{
			m_Data = CreateScope<uint8_t[]>(size);

			std::memcpy(m_Data.get(), data, size);
		}
	}

	FileBuffer::FileBuffer(const FileBuffer& other)
		: m_Size(other.m_Size)
	{
		if (!other.m_Data && other.m_Size != 0)
		{
			m_Data = CreateScope<uint8_t[]>(other.m_Size);
		}
		else if (!other.m_Data && other.m_Size == 0)
		{
			m_Data = nullptr;
		}
		else
		{
			m_Data = CreateScope<uint8_t[]>(other.m_Size);

			std::memcpy(m_Data.get(), other.m_Data.get(), other.m_Size);
		}
	}

	FileBuffer::~FileBuffer()
	{
		m_Data.reset();
	}

}