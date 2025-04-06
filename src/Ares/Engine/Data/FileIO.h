#pragma once

namespace Ares {

	class DataBuffer;

	class FileIO
	{
	public:
		static DataBuffer LoadFile(StringView filepath);

		static bool SaveFile(StringView filepath, const DataBuffer& buffer);

	private:
		FileIO() = default;
	};

}