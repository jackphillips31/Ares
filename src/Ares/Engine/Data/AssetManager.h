#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#define AR_BIND_ASSET_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define AR_BIND_ASSET_STATIC_FN(fn) std::bind(&fn, std::placeholders::_1)

namespace Ares {

	class AssetManager
	{
	public:
		// Public API
		static void Init();
		static void Shutdown();

		template <typename T>
		static void LoadAsset(const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback = nullptr)
		{
			ThreadPool::SubmitTask([filepath, callback]()
			{
				Ref<T> asset = nullptr;
				bool success = false;
				std::string message;

				// Check cache
				{
					std::lock_guard<std::mutex> lock(s_CacheMutex);
					auto& typeMap = s_AssetCache[typeid(T)];
					if (typeMap.find(filepath) != typeMap.end())
					{
						// Asset exists - Get asset and call callback
						asset = static_pointer_cast<T>(typeMap[filepath]);
						AssetLoadedEvent event(filepath, true, asset);
						if (callback)
							QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });

						return;
					}
				}

				// Asset not in cache - Try loading asset
				try
				{
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
						AR_CORE_WARN("Unsupported asset type!");
						throw std::runtime_error("Unsupported asset type!");
					}

					success = true;

					// Add to cache
					{
						std::lock_guard<std::mutex> lock(s_CacheMutex);
						s_AssetCache[typeid(T)][filepath] = asset;
					}
				}
				catch (const std::exception& e)
				{
					message = e.what();
				}

				AssetLoadedEvent event(filepath, success, asset, message);

				// Notify all listeners
				NotifyListeners(filepath, event);

				// Execute callback
				if (callback)
				{
					QueueCallback([callback, event]()
					{
						callback(const_cast<AssetLoadedEvent&>(event));
					});
				}
			});
		}

		template <typename T>
		static Ref<T> GetAsset(const std::string& filepath)
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			auto& typeMap = s_AssetCache[typeid(T)];
			if (typeMap.find(filepath) != typeMap.end())
			{
				return static_pointer_cast<T>(typeMap[filepath]);
			}
			return nullptr;
		}

		static void AddListener(const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback);
		static void AddGlobalListener(std::function<void(AssetLoadedEvent&)> callback);
		static void OnUpdate();

	private:
		// Helpers for callback & listener system
		static void NotifyListeners(const std::string& filepath, AssetLoadedEvent event);
		static void QueueListenerCallback(std::function<void()> callback);
		static void QueueCallback(std::function<void()> callback);

		// Process callbacks & listeners in OnUpdate
		static void ProcessListenerCallbacks();
		static void ProcessCallbacks();

		// Helper to check file extension
		static std::string GetFileExtension(const std::string& filepath);

		// Private loaders
		static Ref<Shader> LoadShader(const std::string& filepath);
		static Ref<Texture2D> LoadTexture2D(const std::string& filepath);

	private:
		// Asset cache
		static std::unordered_map<std::type_index, std::unordered_map<std::string, Ref<Asset>>> s_AssetCache;
		static std::mutex s_CacheMutex;

		// Callback queue
		static std::queue<std::function<void()>> s_CallbackQueue;
		static std::mutex s_CallbackQueueMutex;

		// Event listeners
		static std::unordered_map<std::string, std::vector<std::function<void(AssetLoadedEvent&)>>> s_Listeners;
		static std::vector<std::function<void(AssetLoadedEvent&)>> s_GlobalListeners;
		static std::mutex s_ListenerMutex;

		// Event listener callback queue
		static std::queue<std::function<void()>> s_ListenerCallbackQueue;
		static std::mutex s_ListenerCallbackQueueMutex;
	};

}