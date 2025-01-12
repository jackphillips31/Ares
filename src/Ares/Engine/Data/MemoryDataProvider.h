#pragma once

namespace Ares {

	class DataBuffer;
	using MemoryDataKey = uint32_t;

	class MemoryDataProvider
	{
	public:
		MemoryDataProvider() = delete;

		static MemoryDataKey RegisterData(DataBuffer&& data);
		static MemoryDataKey RegisterData(const void* data, const size_t size);
		static const DataBuffer& GetData(const MemoryDataKey key);
		static void UnregisterData(const MemoryDataKey key);

	private:
		static std::shared_mutex s_Mutex;
		static std::atomic<uint32_t> s_NextMemoryDataKey;
		static std::unordered_map<MemoryDataKey, DataBuffer> s_DataRegistry;
	};

}