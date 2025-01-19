/**
 * @file MemoryDataProvider.h
 * @brief Provides functionality for managing memory data buffers.
 * 
 * @details The MemoryDataProvider class allows the registration, retrieval, and
 * unregistration of memory-based data buffers in the engine. Each data buffer
 * is identified by a unique key, facilitating efficient and organized management.
 */
#pragma once

namespace Ares {

	class DataBuffer;

	/**
	 * @typedef MemoryDataKey
	 * @brief Data Key stored as a 32 bit unsigned integer.
	 */
	using MemoryDataKey = uint32_t;

	/**
	 * @class MemoryDataProvider
	 * @brief Manages memory data buffers using unique keys.
	 * 
	 * @details The MemoryDataProvider class provides a static interface for handling
	 * data buffers in memory. It allows registering data buffers, retrieving them
	 * using a unique key, and unregistering them when no longer needed.
	 */
	class MemoryDataProvider
	{
	public:
		/**
		 * @brief Deleted default constructor to prevent instantiation.
		 */
		MemoryDataProvider() = delete;

		/**
		 * @brief Registers a data buffer and generates a unique key for it.
		 * @param data The data buffer to register (moved into the registry).
		 * @return A unique key associated with the registered data buffer.
		 */
		static MemoryDataKey RegisterData(DataBuffer&& data);

		/**
		 * @brief Registers a raw memory buffer and generates a unique key for it.
		 * @param data Pointer to the raw memory buffer.
		 * @param size Size of the memory buffer in bytes.
		 * @return A unique key associated with the registered memory buffer.
		 */
		static MemoryDataKey RegisterData(const void* data, const size_t size);

		/**
		 * @brief Retrieves a data buffer by its unique key.
		 * @param key The unique key associated with the data buffer.
		 * @return A reference to the data buffer.
		 * @note Returns an empty DataBuffer object if the key does not exist.
		 */
		static const DataBuffer& GetData(const MemoryDataKey key);

		/**
		 * @brief Unregisters a data buffer by its unique key.
		 * @param key The unique key of the data buffer to unregister.
		 * 
		 * @details Removes the data buffer from the registry, freeing associated resources.
		 */
		static void UnregisterData(const MemoryDataKey key);

	private:
		static std::shared_mutex s_Mutex;
		static std::atomic<uint32_t> s_NextMemoryDataKey;
		static std::unordered_map<MemoryDataKey, DataBuffer> s_DataRegistry;
	};

}