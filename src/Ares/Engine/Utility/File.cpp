#include <arespch.h>
#include "Engine/Utility/File.h"

namespace Ares::Utility {

	std::string GetFileExtension(const std::string& filepath)
	{
		size_t dotPos = filepath.rfind(".");
		if (dotPos == std::string::npos)
		{
			return "";
		}
		return filepath.substr(dotPos + 1);
	}

	std::string GetFilename(const std::string& filepath)
	{
		size_t lastSlashPos = filepath.find_last_of("/\\");
		if (lastSlashPos == std::string::npos)
		{
			return filepath;
		}

		return filepath.substr(lastSlashPos + 1);
	}

	std::string GetFilenameNoExt(const std::string& filepath)
	{
		size_t lastSlashPos = filepath.find_last_of("/\\");
		if (lastSlashPos == std::string::npos)
		{
			lastSlashPos = -1;
		}

		size_t lastDotPos = filepath.find_last_of(".");
		if (lastDotPos == std::string::npos || lastDotPos < lastSlashPos)
		{
			return filepath.substr(lastSlashPos + 1);
		}

		return filepath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
	}

}