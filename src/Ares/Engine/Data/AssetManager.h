#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Data/Asset.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

namespace Ares {

	class AssetManager
	{
	public:
		// Public API
		template <typename T>
		static std::future<Ref<T>> GetAsset(const std::string& filepath)
		{
			return ThreadPool::SubmitTask([filepath]() -> Ref<T>
			{
				{
					// Check cache
					std::lock_guard<std::mutex> lock(s_CacheMutex);
					auto& typeMap = s_AssetCache[typeid(T)];
					if (typeMap.find(filepath) != typeMap.end())
					{
						return std::static_pointer_cast<T>(typeMap[filepath]);
					}
				}

				// Load the asset
				Ref<T> asset;
				if constexpr (std::is_same<T, Shader>::value)
				{
					asset = LoadShader(filepath);
				}
				else if constexpr (std::is_same<T, Texture2D>::value)
				{
					asset = LoadTexture2D(filepath);
				}
				else
				{
					AR_CORE_ASSERT(false, "Unsupported asset type for template GetAsset!");
				}

				{
					// Add to cache
					std::lock_guard<std::mutex> lock(s_CacheMutex);
					s_AssetCache[typeid(T)][filepath] = asset;
				}

				return asset;
			});
		}

	private:
		// Private loaders
		static Ref<Shader> LoadShader(const std::string& filepath);
		static Ref<Texture2D> LoadTexture2D(const std::string& filepath);

		// Asset cache
		static std::unordered_map<std::type_index, std::unordered_map<std::string, Ref<Asset>>> s_AssetCache;
		static std::mutex s_CacheMutex;

		// Helper to check file extension
		static std::string GetFileExtension(const std::string& filepath);
	};

}