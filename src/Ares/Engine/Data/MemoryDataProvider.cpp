#include <arespch.h>
#include "Engine/Data/MemoryDataProvider.h"

#include "Engine/Data/DataBuffer.h"

namespace Ares {

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