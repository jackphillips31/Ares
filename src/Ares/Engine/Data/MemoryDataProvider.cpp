#include <arespch.h>
#include "Engine/Data/MemoryDataProvider.h"

#include "Engine/Data/DataBuffer.h"

namespace Ares {

	namespace Internal {

		MemoryDataProviderSys::MemoryDataProviderSys()
			: m_NextDataKey(1)
		{
		}

		MemoryDataProviderSys::~MemoryDataProviderSys()
		{
			m_DataRegistry.clear();
		}

		MemoryDataKey MemoryDataProviderSys::RegisterData(DataBuffer&& data)
		{
			std::unique_lock lock(m_Mutex);
			MemoryDataKey key = m_NextDataKey++;
			m_DataRegistry[key] = eastl::move(data);
			return key;
		}

		MemoryDataKey MemoryDataProviderSys::RegisterData(const void* data, const size_t& size)
		{
			std::unique_lock lock(m_Mutex);
			MemoryDataKey key = m_NextDataKey++;
			m_DataRegistry[key] = DataBuffer(data, size);
			return key;
		}

		bool MemoryDataProviderSys::UnregisterData(MemoryDataKey& key)
		{
			std::unique_lock lock(m_Mutex);
			auto it = m_DataRegistry.find(key);
			if (it != m_DataRegistry.end())
			{
				m_DataRegistry.erase(it);
				key = 0;
				return true;
			}
			return false;
		}

		const DataBuffer& MemoryDataProviderSys::GetDataBuffer(const MemoryDataKey& key)
		{
			std::shared_lock lock(m_Mutex);
			return m_DataRegistry[key];
		}

	}

	std::shared_mutex MemoryDataProvider::s_Mutex;
	std::atomic<uint32_t> MemoryDataProvider::s_NextMemoryDataKey{ 1 };
	std::unordered_map<MemoryDataKey, DataBuffer> MemoryDataProvider::s_DataRegistry;

	MemoryDataKey MemoryDataProvider::RegisterData(DataBuffer&& data)
	{
		std::unique_lock lock(s_Mutex);
		MemoryDataKey key = s_NextMemoryDataKey++;
		s_DataRegistry[key] = std::move(data);
		return key;
	}

	MemoryDataKey MemoryDataProvider::RegisterData(const void* data, const size_t size)
	{
		std::unique_lock lock(s_Mutex);
		MemoryDataKey key = s_NextMemoryDataKey++;
		s_DataRegistry[key] = DataBuffer(data, size);
		return key;
	}

	const DataBuffer& MemoryDataProvider::GetData(const MemoryDataKey key)
	{
		std::shared_lock lock(s_Mutex);
		return s_DataRegistry[key];
	}

	void MemoryDataProvider::UnregisterData(const MemoryDataKey key)
	{
		std::unique_lock lock(s_Mutex);
		auto it = s_DataRegistry.find(key);
		if (it != s_DataRegistry.end())
			s_DataRegistry.erase(it);
	}

}