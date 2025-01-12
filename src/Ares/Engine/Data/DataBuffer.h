#pragma once

namespace Ares {

	class FileIO;

	class DataBuffer
	{
	public:
		DataBuffer() : m_Data(nullptr), m_Size(0) {}
		DataBuffer(const void* data, const size_t size);
		~DataBuffer();

		// Move methods
		DataBuffer(DataBuffer&& other) noexcept;
		DataBuffer& operator=(DataBuffer&& other) noexcept;

		// Delete copy methods
		DataBuffer(const DataBuffer&) = delete;
		DataBuffer& operator=(const DataBuffer&) = delete;

		// Getters
		inline const void* GetBuffer() const { std::shared_lock lock(m_Mutex); return m_Data; }
		inline size_t GetSize() const { std::shared_lock lock(m_Mutex); return m_Size; }

		// Setters
		void ResetData(const void* data, const size_t size);

		// Operators
		inline explicit operator bool() const
		{
			std::shared_lock lock(m_Mutex);
			return m_Data != nullptr && m_Size > 0;
		}

	private:
		friend class FileIO;
		inline void* SetBuffer() const { return m_Data; }

	private:
		mutable std::shared_mutex m_Mutex;
		void* m_Data;
		size_t m_Size;
	};

}