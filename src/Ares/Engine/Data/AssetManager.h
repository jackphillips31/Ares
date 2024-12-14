#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Events/EventQueue.h"
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

		// Load asset functions
		template <typename T>
		static void LoadAsset(const std::string& name, const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback = nullptr);

		template <typename T>
		static void LoadAsset(const std::string& name, const std::vector<Ref<Shader>>& shaders, std::function<void(AssetLoadedEvent&)> callback = nullptr);

		// Getter functions
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
		static std::vector<std::pair<std::string, std::string>> GetCompleteList();

		// Listener functions & OnUpdate
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

		// Private loaders
		static Ref<VertexShader> LoadVertexShader(const std::string& name, const std::string& filepath);
		static Ref<FragmentShader> LoadFragmentShader(const std::string& name, const std::string& filepath);
		static Ref<Texture2D> LoadTexture2D(const std::string& name, const std::string& filepath);

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