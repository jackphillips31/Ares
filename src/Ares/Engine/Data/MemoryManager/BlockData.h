#pragma once

namespace Ares::Internal {

	struct BlockData
	{
	public:
		uint32_t GetSize() const;
		bool GetAlloc() const;

		void SetSize(const uint32_t& size);
		void SetAlloc(const bool& isAlloc);

		inline bool operator==(const BlockData& other) { return m_Data == other.m_Data; }
		inline bool operator!=(const BlockData& other) { return m_Data != other.m_Data; }

	private:
		uint32_t m_Data;
	};

}