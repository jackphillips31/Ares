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

		// Stage asset function
		template <typename AssetType>
		static Ref<Asset> StageAsset(const std::string& name, const std::string& filepath = "", const std::vector<uint32_t>& dependencies = {});
		template <typename AssetType>
		inline static Ref<Asset> StageAsset(const std::string& name, const std::string& filepath) { return StageAsset<AssetType>(name, filepath, {}); }
		template <typename AssetType>
		inline static Ref<Asset> StageAsset(const std::string& name, const std::vector<uint32_t>& dependencies) { return StageAsset<AssetType>(name, "", dependencies); }

		// Load asset functions
		template <typename AssetType>
		static void LoadAsset(const std::string& name, const std::string& filepath, const AssetCallbackFn = nullptr);
		template <typename AssetType>
		static void LoadAsset(const std::string& name, const std::vector<Ref<Asset>>& shaders, const AssetCallbackFn = nullptr);

		// Getter functions
		static Ref<Asset> GetAsset(const std::string& name);
		static std::vector<Ref<Asset>> GetCompleteList();

		// Listener functions & OnUpdate
		static const AssetListener AddListener(const std::string& name, std::function<void(Event&)> callback);
		static const AssetListener AddListener(std::function<void(Event&)> callback);
		static void RemoveListener(AssetListener& listenerId);
		static void OnUpdate();

	private:
		// Helpers for callback & listener system
		static void DispatchAssetEvent(const Ref<Asset>& asset, const std::string& message = "");
		static void QueueListenerCallback(std::function<void()> callback);
		static void QueueCallback(std::function<void()> callback);

		// Process callbacks & listeners in OnUpdate
		static void ProcessListenerCallbacks();
		static void ProcessCallbacks();

		// Private loaders
		static Ref<VertexShader> LoadVertexShader(const std::string& name, const std::string& filepath);
		static Ref<FragmentShader> LoadFragmentShader(const std::string& name, const std::string& filepath);
		static Ref<Texture2D> LoadTexture2D(const std::string& name, const std::string& filepath);
		static Ref<ShaderProgram> LoadShaderProgram(const std::string& name, const std::string& filepath);

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

}