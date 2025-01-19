/**
 * @file AssetManager.h
 * @brief Declaration of the AssetManager class for managing assets, including staging, loading,
 * unloading, unstaging, and listener systems.
 */
#pragma once

namespace Ares {

	class Asset;
	class Event;
	struct RawData;

	/**
	 * @typedef AssetListener
	 * @brief Listener ID stored as a 32 bit unsigned integer.
	 */
	using AssetListener = uint32_t;
	using MemoryDataKey = uint32_t;

	/**
	 * @class AssetManager
	 * @brief A static class for managing assets, including staging, loading, unloading, unstaging,
	 * and event listeners.
	 */
	class AssetManager
	{
	public:
		/**
		 * @typedef AssetCallbackFn
		 * @brief The callback function used in the Load method. It is a void function that takes a
		 * Ref to an Asset object as the argument.
		 */
		using AssetCallbackFn = std::function<void(Ref<Asset>)>;
		/**
		 * @typedef AssetListenerCallbackFn
		 * @brief The callback function used for listener callbacks. It is a void function that takes
		 * a reference to an Event as the argument.
		 */
		using AssetListenerCallbackFn = std::function<void(Event&)>;
	public:
		/**
		 * @brief Initializes the AssetManager.
		 */
		static void Init();

		/**
		 * @brief Shuts down the AssetManager and cleans up resources.
		 */
		static void Shutdown();

		/**
		 * @brief Stages an asset for loading.
		 * 
		 * @tparam AssetType The type of the asset.
		 * @param name The name of the asset.
		 * @param filepath The filepath of the asset.
		 * @param dependencies A vector of dependencies required by the asset.
		 * @param dataKey A key for memory data.
		 * @return A Ref to the staged Asset object.
		 */
		template <typename AssetType>
		static Ref<Asset> Stage(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies, const MemoryDataKey dataKey);
		
		/**
		 * @brief Stages an asset for loading using a filepath.
		 * 
		 * @tparam AssetType The type of the asset.
		 * @param filepath The filepath of the asset.
		 * @return A Ref to the staged Asset object.
		 */
		template <typename AssetType>
		inline static Ref<Asset> Stage(const std::string& name, const std::string& filepath) { return Stage<AssetType>(name, filepath, {}, 0); }
		
		/**
		 * @brief Stages an asset for loading using dependencies.
		 * 
		 * @tparam AssetType The type of the asset.
		 * @param dependencies A vector of dependencies required by the asset.
		 * @return A Ref to the staged Asset object.
		 */
		template <typename AssetType>
		inline static Ref<Asset> Stage(const std::string& name, const std::vector<Ref<Asset>>& dependencies) { return Stage<AssetType>(name, "", dependencies, 0); }
		
		/**
		 * @brief Stages an asset for loading using a MemoryDataKey.
		 * 
		 * @tparam AssetType The type of the asset.
		 * @param dataKey A key for memory data.
		 * @return A Ref to the staged Asset object.
		 */
		template <typename AssetType>
		inline static Ref<Asset> Stage(const std::string& name, const MemoryDataKey dataKey) { return Stage<AssetType>(name, "", {}, dataKey); }
		
		/**
		 * @brief Removes an asset completely from the AssetManager.
		 * 
		 * @param asset The asset to unstage.
		 */
		static void Unstage(const Ref<Asset>& asset);

		/**
		 * @brief Loads an asset asynchronously.
		 * 
		 * @param asset The asset to load.
		 * @param callback An optional callback function to execute after loading.
		 */
		static void Load(const Ref<Asset>& asset, AssetCallbackFn&& = nullptr);

		/**
		 * @brief Loads multiple assets asynchronously.
		 * 
		 * @param assets A vector of assets to load.
		 * @param callback An optional callback function to execute whenever an asset is loaded.
		 */
		static void Load(const std::vector<Ref<Asset>>& assets, AssetCallbackFn&& = nullptr);

		/**
		 * @brief Unloads an asset.
		 * 
		 * @param asset The asset to unload.
		 */
		static void Unload(const Ref<Asset>& asset);

		/**
		 * @brief Retrieves an asset by its name.
		 * 
		 * @param name The name of the asset.
		 * @return A Ref to the Asset object.
		 */
		static Ref<Asset> GetAsset(const std::string& name);

		/**
		 * @brief Retrieves an asset by its ID.
		 * 
		 * @param assetId The ID of the asset.
		 * @return A Ref to the Asset object.
		 */
		static Ref<Asset> GetAsset(const uint32_t& assetId);

		/**
		 * @brief Retrieves a complete list of all assets.
		 * 
		 * @return A vector of Ref's to all Asset objects.
		 */
		static std::vector<Ref<Asset>> GetCompleteList();

		/**
		 * @brief Adds a listener to an Asset by name.
		 * 
		 * @param name The name of the Asset object to be listened to.
		 * @param callback The callback function of the listener.
		 * @return The ID of the added listener.
		 */
		static const AssetListener AddListener(const std::string& name, AssetListenerCallbackFn&& callback);

		/**
		 * @brief Adds a global listener that listens to all Asset objects.
		 * 
		 * @param callback The callback function of the listener.
		 * @return The ID of the added listener.
		 */
		static const AssetListener AddListener(AssetListenerCallbackFn&& callback);

		/**
		 * @brief Removes an event listener by its ID.
		 * 
		 * @param listenerId The ID of the listener to remove.
		 */
		static void RemoveListener(AssetListener& listenerId);

		/**
		 * @brief Processes queued events and callbacks.
		 */
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
		static void LoadRawAsset(const Ref<Asset>& asset, AssetCallbackFn&& callback);

		// Private hash function
		static const size_t GetHash(const std::type_index& type, const std::string& filepath, const std::vector<uint32_t>& dependencies, const MemoryDataKey dataKey);
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
		static std::unordered_map<std::string, std::unordered_map<uint32_t, AssetListenerCallbackFn>> s_Listeners;
		static std::unordered_map<uint32_t, std::string> s_ListenerNameMap;
		static std::unordered_map<uint32_t, AssetListenerCallbackFn> s_GlobalListeners;
		static std::mutex s_ListenerMutex;

		// Event listener callback queue
		static std::queue<std::function<void()>> s_ListenerCallbackQueue;
		static std::mutex s_ListenerCallbackQueueMutex;
	};

}