#pragma once

namespace Ares {

	class FileBuffer
	{
	public:
		FileBuffer(const void* data, size_t size);
		FileBuffer(const FileBuffer& other);
		~FileBuffer();

		inline const void* GetData() const { return m_Data.get(); }
		inline size_t GetSize() const { return m_Size; }

		inline bool IsEmpty() const { return m_Data == nullptr || m_Size == 0; }

	private:
		Scope<uint8_t[]> m_Data;
		size_t m_Size = 0;
	};

}