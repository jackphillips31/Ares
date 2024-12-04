#pragma once

#include "Engine/Data/FileBuffer.h"

namespace Ares {

	class FileIO
	{
	public:
		static FileBuffer LoadFile(const std::string& filepath);

		static bool SaveFile(const std::string& filepath, const FileBuffer& buffer);

	private:
		FileIO() = default;
	};

}