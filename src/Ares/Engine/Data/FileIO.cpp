#include <arespch.h>

#include "Engine/Data/FileIO.h"

namespace Ares {

	FileBuffer FileIO::LoadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::in);
		if (!file)
		{
			AR_CORE_WARN("Failed to open file for reading: '{}'", filepath);
			return FileBuffer(nullptr, 0);
		}
		else
		{
			file.seekg(0, std::ios::end);
			size_t fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			Scope<uint8_t[]> buffer = CreateScope<uint8_t[]>(fileSize);
			if (!file.read(reinterpret_cast<char*>(buffer.get()), fileSize))
			{
				AR_CORE_WARN("Failed to read from file: '{}'", filepath);
				return FileBuffer(nullptr, 0);
			}
			else
			{
				return FileBuffer(buffer.get(), fileSize);
			}
		}
	}

	bool FileIO::SaveFile(const std::string& filepath, const FileBuffer& buffer)
	{
		std::ofstream file(filepath, std::ios::binary | std::ios::in);
		if (!file)
		{
			AR_CORE_WARN("Failed to open file for writing: '{}'", filepath);
			return false;
		}
		else
		{
			file.write(reinterpret_cast<const char*>(buffer.GetData()), buffer.GetSize());
			file.close();
			return true;
		}
	}

}