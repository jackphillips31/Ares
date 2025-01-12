#pragma once

namespace Ares {

	class DataBuffer;

	class FileIO
	{
	public:
		static DataBuffer LoadFile(const std::string& filepath);

		static bool SaveFile(const std::string& filepath, const DataBuffer& buffer);

	private:
		FileIO() = default;
	};

}