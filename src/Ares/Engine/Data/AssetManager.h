#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#define AR_BIND_ASSET_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define AR_BIND_ASSET_STATIC_FN(fn) std::bind(&fn, std::placeholders::_1)

namespace Ares {

	using AssetListener = uint32_t;

	class AssetManager
	{
	private:
		using AssetCallbackFn = std::function<void(Ref<Asset>)>;
		using ListenerCallbackFn = std::function<void(Event&)>;
	public:
		// Public API
		static void Init();
		static void Shutdown();

		// Stage / Unstage methods
		template <typename AssetType>
		static Ref<Asset> Stage(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies);
		template <typename AssetType>
		inline static Ref<Asset> Stage(const std::string& name, const std::string& filepath) { return Stage<AssetType>(name, filepath, {}); }
		template <typename AssetType>
		inline static Ref<Asset> Stage(const std::string& name, const std::vector<Ref<Asset>>& dependencies) { return Stage<AssetType>(name, "", dependencies); }
		static void Unstage(const Ref<Asset>& asset);

		// Load / Unload methods
		static void Load(const Ref<Asset>& asset, const AssetCallbackFn = nullptr);
		static void Load(const std::vector<Ref<Asset>>& assets, const AssetCallbackFn = nullptr);
		static void Unload(const Ref<Asset>& asset);

		// Getter functions
		static Ref<Asset> GetAsset(const std::string& name);
		static Ref<Asset> GetAsset(const uint32_t& assetId);
		static std::vector<Ref<Asset>> GetCompleteList();

		// Listener functions & OnUpdate
		static const AssetListener AddListener(const std::string& name, std::function<void(Event&)> callback);
		static const AssetListener AddListener(std::function<void(Event&)> callback);
		static void RemoveListener(AssetListener& listenerId);
		static void OnUpdate();

	private:
		// Helpers for callback & listener system
		template <typename AssetEventType>
		static void DispatchAssetEvent(const Ref<Asset>& asset, const std::string& message = "");
		static void QueueListenerCallback(std::function<void()> callback);
		static void QueueCallback(std::function<void()> callback);

		// Process callbacks & listeners in OnUpdate
		static void ProcessListenerCallbacks();
		static void ProcessCallbacks();

		// Private loaders
		static void LoadRawAsset(const Ref<Asset>& asset, const AssetCallbackFn callback = nullptr);
		static Ref<VertexShader> LoadVertexShader(const Ref<Asset>& asset);
		static Ref<FragmentShader> LoadFragmentShader(const Ref<Asset>& asset);
		static Ref<Texture2D> LoadTexture2D(const Ref<Asset>& asset);
		static Ref<ShaderProgram> LoadShaderProgram(const Ref<Asset>& asset);

		// Private hash function
		static const size_t GetHash(const std::type_index& type, const std::string& filepath, const std::vector<uint32_t>& dependencies = {});
		static Ref<Asset> FindExistingAsset(const size_t& contentHash);

	private:
		// Asset cache
		static std::unordered_map<uint32_t, Ref<Asset>> s_AssetCache;
		static std::mutex s_CacheMutex;
		static std::atomic<uint32_t> s_NextAssetId;

		// Secondary lookup maps
		static std::unordered_map<std::string, uint32_t> s_NameIdMap;
		static std::mutex s_NameIdMutex;
		static std::unordered_map<size_t, uint32_t> s_HashIdMap;
		static std::mutex s_HashIdMutex;
		static std::unordered_map<std::type_index, uint32_t> s_TypeIdMap;
		static std::mutex s_TypeIdMutex;

		// Callback queue
		static std::queue<std::function<void()>> s_CallbackQueue;
		static std::mutex s_CallbackQueueMutex;

		static std::atomic<uint32_t> s_NextListenerId;
		static std::unordered_map<std::string, std::unordered_map<uint32_t, ListenerCallbackFn>> s_Listeners;
		static std::unordered_map<uint32_t, std::string> s_ListenerNameMap;
		static std::unordered_map<uint32_t, ListenerCallbackFn> s_GlobalListeners;
		static std::mutex s_ListenerMutex;

		// Event listener callback queue
		static std::queue<std::function<void()>> s_ListenerCallbackQueue;
		static std::mutex s_ListenerCallbackQueueMutex;
	};

	template <typename AssetEventType>
	void AssetManager::DispatchAssetEvent(const Ref<Asset>& asset, const std::string& message)
	{
		// Make sure AssetEventType is an asset event
		if (!std::is_base_of_v<AssetBaseEvent, AssetEventType>)
		{
			AR_CORE_ASSERT(false, "Tried to dispatch non-asset event: {}", typeid(AssetEventType).name());
		}

		// Create event
		AssetEventType tempEvent(asset, message);
		Ref<AssetEventType> event = CreateRef<AssetEventType>(tempEvent);

		// Dispatch to event queue
		EventQueue::Dispatch<AssetEventType>(tempEvent);

		// Dispatch to asset listeners
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);

			// Queue name-specific listeners
			if (s_Listeners.find(asset->GetName()) != s_Listeners.end())
			{
				for (auto& listener : s_Listeners[asset->GetName()])
					QueueListenerCallback([func = listener.second, event]() { func(*event); });
			}

			// Queue global listeners
			for (auto& globalListener : s_GlobalListeners)
				QueueListenerCallback([func = globalListener.second, event]() { func(*event); });
		}
	}

}