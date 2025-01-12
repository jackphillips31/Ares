#include <arespch.h>
#include "Engine/Data/FileIO.h"

#include "Engine/Data/DataBuffer.h"

namespace Ares {

	DataBuffer FileIO::LoadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::in);
		if (!file)
		{
			AR_CORE_WARN("Failed to open file for reading: '{}'", filepath);
			//return FileBuffer({ nullptr, 0 });
			return DataBuffer(nullptr, 0);
		}
		else
		{
			file.seekg(0, std::ios::end);
			size_t fileSize = static_cast<size_t>(file.tellg());
			file.seekg(0, std::ios::beg);

			//FileBuffer result({ nullptr, fileSize });
			void* tempBuffer = new uint8_t[fileSize];
			DataBuffer result(nullptr, fileSize);

			if (!file.read(reinterpret_cast<char*>(result.SetBuffer()), fileSize))
			{
				AR_CORE_WARN("Failed to read from file: '{}'", filepath);
				return DataBuffer(nullptr, 0);
			}
			else
			{
				return result;
			}
		}
	}

	bool FileIO::SaveFile(const std::string& filepath, const DataBuffer& buffer)
	{
		std::ofstream file(filepath, std::ios::binary | std::ios::in);
		if (!file)
		{
			AR_CORE_WARN("Failed to open file for writing: '{}'", filepath);
			return false;
		}
		else
		{
			file.write(reinterpret_cast<const char*>(buffer.GetBuffer()), buffer.GetSize());
			file.close();
			return true;
		}
	}

}