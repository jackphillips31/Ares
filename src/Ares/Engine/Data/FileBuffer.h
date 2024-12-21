#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Utility.h"

namespace Ares {

	class FileBuffer
	{
	public:
		FileBuffer(const RawData& data);
		FileBuffer(const FileBuffer& other) = delete;
		FileBuffer(FileBuffer&& other) noexcept;
		~FileBuffer();

		inline const void* GetBuffer() const { return m_Data.get(); }
		inline size_t GetSize() const { return m_Size; }

		inline bool IsEmpty() const { return m_Data == nullptr || m_Size == 0; }

	private:
		inline void* SetBuffer() const { return m_Data.get(); }
		friend class FileIO;

	private:
		Scope<uint8_t[]> m_Data;
		size_t m_Size = 0;
	};

}