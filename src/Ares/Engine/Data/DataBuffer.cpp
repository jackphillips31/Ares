#include <arespch.h>
#include "Engine/Data/DataBuffer.h"

namespace Ares {

	DataBuffer::DataBuffer(const void* data, const size_t size)
		: m_Data(new uint8_t[size]), m_Size(size)
	{
		if (data != nullptr)
			std::memcpy(m_Data, data, size);
	}

	DataBuffer::~DataBuffer()
	{
		std::unique_lock lock(m_Mutex);
		delete[] static_cast<uint8_t*>(m_Data);
		m_Size = 0;
	}

	DataBuffer::DataBuffer(DataBuffer&& other) noexcept
	{
		std::unique_lock lock(other.m_Mutex);
		m_Data = other.m_Data;
		m_Size = other.m_Size;
		other.m_Data = nullptr;
		other.m_Size = 0;
	}

	DataBuffer& DataBuffer::operator=(DataBuffer&& other) noexcept
	{
		std::unique_lock lock1(m_Mutex);
		std::unique_lock lock2(other.m_Mutex);
		m_Data = other.m_Data;
		m_Size = other.m_Size;
		other.m_Data = nullptr;
		other.m_Size = 0;
		return *this;
	}

	void DataBuffer::ResetData(const void* data, const size_t size)
	{
		std::unique_lock lock(m_Mutex);
		delete[] static_cast<uint8_t*>(m_Data);
		m_Data = new uint8_t[size];
		m_Size = size;

		std::memcpy(m_Data, data, size);
	}

}