#pragma once

namespace Ares {

	struct RawData
	{
		const void* Data;
		size_t Size;

		explicit operator bool() const
		{
			return Data != nullptr && Size != 0;
		}

		RawData()
			: Data(nullptr), Size(0)
		{
		}

		RawData(const void* data, const size_t& size)
			: Data(data), Size(size)
		{
		}

		RawData(const RawData& other)
			: Data(other.Data), Size(other.Size)
		{
		}

		RawData& operator=(const RawData& other)
		{
			this->Data = other.Data;
			this->Size = other.Size;
			return *this;
		}
	};

}