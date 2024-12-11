#include <arespch.h>

#include "Engine/Core/ThreadPool.h"
#include "Engine/Data/FileIO.h"

#include "Engine/Data/AssetManager.h"

namespace Ares {

	std::unordered_map<std::type_index, std::unordered_map<std::string, Ref<Asset>>> AssetManager::s_AssetCache;
	std::mutex AssetManager::s_CacheMutex;

	Ref<Shader> AssetManager::LoadShader(const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			AR_CORE_ASSERT(false, "Failed to laod shader file: {}", filepath);
		}

		return Shader::Create(filepath, fileBuffer);
	}

	Ref<Texture2D> AssetManager::LoadTexture2D(const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			AR_CORE_ASSERT(false, "Failed to load texture2D file: {}", filepath);
		}

		return Texture2D::Create(filepath, fileBuffer);
	}

	std::string AssetManager::GetFileExtension(const std::string& filepath)
	{
		size_t dotPos = filepath.find_last_of(".");
		if (dotPos == std::string::npos)
		{
			return "";
		}
		return filepath.substr(dotPos);
	}

}