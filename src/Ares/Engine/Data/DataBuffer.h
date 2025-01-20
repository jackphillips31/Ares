/**
 * @file DataBuffer.h
 * @brief Defines the DataBuffer class for managing raw memory buffers.
 * 
 * @details The DataBuffer class is a utility for handling raw memory data, providing a
 * thread-safe interface for accessing and modifying the buffer. It is designed for
 * scenarios where memory buffers need to be shared or managed dynamically.
 */
#pragma once

namespace Ares {

	class FileIO;

	/**
	 * @class DataBuffer
	 * @brief A thread-safe utility class for managing raw memory buffers.
	 * 
	 * @details The DataBuffer class provides functionality for storing, accessing, and
	 * resetting raw memory data. It ensures thread safety using a shared mutex and
	 * support move semantics for efficient buffer management.
	 */
	class DataBuffer
	{
	public:
		/**
		 * @brief Default constructor. Initializes an empty buffer.
		 */
		DataBuffer() : m_Data(nullptr), m_Size(0) {}

		/**
		 * @brief Constructs a DataBuffer with the given data and size.
		 * 
		 * @param data Pointer to the raw memory data.
		 * @param size Size of the memory buffer in bytes.
		 */
		DataBuffer(const void* data, const size_t size);

		/**
		 * @brief Destructor. Cleans up the allocated memory buffer.
		 */
		~DataBuffer();

		/**
		 * @brief Move constructor. Transfers ownership of the buffer.
		 * 
		 * @param other The DataBuffer to move from.
		 */
		DataBuffer(DataBuffer&& other) noexcept;

		/**
		 * @brief Move assignment operator. Transfers ownership of the buffer.
		 * 
		 * @param other The DataBuffer to move from.
		 * @return A reference to the assigned DataBuffer.
		 */
		DataBuffer& operator=(DataBuffer&& other) noexcept;

		/**
		 * @brief Deleted copy constructor. DataBuffer cannot be copied.
		 */
		DataBuffer(const DataBuffer&) = delete;

		/**
		 * @brief Deleted copy assignment operator. DataBuffer cannot be copied.
		 */
		DataBuffer& operator=(const DataBuffer&) = delete;

		/**
		 * @brief Retrieves a const pointer to the buffer's data.
		 * 
		 * @return A pointer to the buffer's data.
		 */
		inline const void* GetBuffer() const { std::shared_lock lock(m_Mutex); return m_Data; }

		/**
		 * @brief Retrieves the size of the buffer.
		 * 
		 * @return The size of the buffer in bytes.
		 */
		inline size_t GetSize() const { std::shared_lock lock(m_Mutex); return m_Size; }

		/**
		 * @brief Resets the buffer with new data and size.
		 * 
		 * @param data Pointer to the new memory data.
		 * @param size Size of the new memory buffer in bytes.
		 */
		void ResetData(const void* data, const size_t size);

		/**
		 * @brief Checks if the buffer contains valid data.
		 * 
		 * @return `true` if the buffer is non-null and has a positive size; otherwise, `false`.
		 */
		inline explicit operator bool() const
		{
			std::shared_lock lock(m_Mutex);
			return m_Data != nullptr && m_Size > 0;
		}

	private:
		friend class FileIO;

		/**
		 * @brief Provides mutable access to the buffer for internal use.
		 * 
		 * @return A pointer to the buffer's data.
		 */
		inline void* SetBuffer() const { return m_Data; }

	private:
		mutable std::shared_mutex m_Mutex;	///< Mutex for thread-safe access to the buffer.
		void* m_Data;						///< Pointer to the raw memory buffer.
		size_t m_Size;						///< Size of the memory buffer in bytes.
	};

}