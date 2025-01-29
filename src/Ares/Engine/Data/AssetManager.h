/**
 * @file AssetManager.h
 * @brief Declaration of the AssetManager class for managing assets, including staging, loading,
 * unloading, unstaging, and listener systems.
 * 
 * @details The AssetManager is a static class responsible for handling all asset-related
 * operations within the Ares engine.
 * 
 * ### Functionalities:
 *  - Staging assets: Preparing assets for loading by specifying metadata.
 *  - Loading assets: Asynchronously loading assets into memory with optional callback support.
 *  - Unloading assets: Removing assets from memory to free resources.
 *  - Managing dependencies: Ensuring assets can reference other required assets.
 *  - Event-driven systems: Allowing listeners and callbacks for asset-related events.
 * 
 * ### Key Features:
 *  - Thread-Safe: Utilizes mutexes and atomic operations to ensure thread safety.
 *  - Caching: Maintains an asset cache for efficient retrieval by name or ID.
 *  - Callback System: Enables event-driven operations through listener callbacks.
 *  - Versatile Asset Handling: Supports various asset types through templates and dynamic indexing.
 * 
 * This class is integral to the resource management system in the engine, ensuring assets
 * are efficiently handled throughout the application lifecycle.
 */
#pragma once
#include <EASTL/hash_map.h>
#include <EASTL/hash_set.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/queue.h>

#include "Engine/Core/System.h"
#include "Engine/Core/Utility.h"
#include "Engine/Data/Asset.h"
#include "Engine/Data/MemoryDataProvider.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/EventQueue.h"

namespace Ares {

	class Application;
	class Asset;
	class AssetStagedEvent;
	class Event;
	class Timestep;
	struct RawData;

	/**
	 * @typedef AssetListener
	 * @brief Listener ID stored as a 32 bit unsigned integer.
	 */
	using AssetListener = uint32_t;
	using MemoryDataKey = uint32_t;

	namespace Systems {

		class ThreadPool;

		/**
		 * @class AssetManager
		 * @brief Manages the loading, unloading, and staging of assets.
		 * 
		 * @details The AssetManager handles asset lifecycle operations, including staging, loading, and
		 * unloading.
		 * Assets can be loaded from files or from memory, and loading can be performed on a separate thread
		 * if a [ThreadPool](#Systems::ThreadPool) is provided. If no [ThreadPool](#Systems::ThreadPool) is
		 * provided, loading occurs on the main thread.
		 * 
		 * The AssetManager created by the Application has a ThreadPool provided to it by the Application.
		 * Asset loading will be asynchronous as long as the thread count provided to ApplicationSettings is
		 * greater than 0.
		 * 
		 * **Example Usage with Application**:
		 * ```cpp
		 * #include <Ares.h>
		 * 
		 * using namespace Ares;
		 * 
		 * Systems::AssetManager* assetManager = Application::Get().GetSystem<Systems::AssetManager>();
		 * 
		 * assetManager->Load(assetManager->Stage<ShaderProgram>("path/to/shader.glsl"), [](const Ref<Asset>& asset)
		 * {
		 *     if (asset->GetState() == AssetState::Loaded)
		 *     {
		 *         // Do something with asset
		 *     }
		 * });
		 * ```
		 * 
		 * **Example Usage without Application**:
		 * ```cpp
		 * #include <Engine/Core/ThreadPool.h>
		 * #include <Engine/Data/AssetManager.h>
		 * 
		 * using namespace Ares;
		 * 
		 * Scope<Systems::ThreadPool> threadPool = Systems::ThreadPool::Create(4); // 4 threads
		 * Scope<Systems::AssetManager> assetManager = Systems::AssetManager::Create(threadPool.get());
		 * 
		 * assetManager->Load(assetManager->Stage<ShaderProgram>("path/to/shader.glsl"), [](const Ref<Asset>& asset)
		 * {
		 *     if (asset->GetState() == AssetState::Loaded)
		 *     {
		 *         // Do something with asset
		 *     }
		 * });
		 * ```
		 * 
		 * **How to Load From Memory**:
		 * ```cpp
		 * #include <Ares.h>
		 * 
		 * using namespace Ares;
		 * 
		 * Systems::AssetManager* assetManager = Application::Get().GetSystem<Systems::AssetManager>();
		 * 
		 * assetManager->Load(assetManager->Stage<ShaderProgram>("MyShaderProgram", dataPtr, dataSize), [](const Ref<Asset>& asset)
		 * {
		 *     if (asset->GetState() == AssetState::Loaded)
		 *     {
		 *         // Do something with asset
		 *     }
		 * }
		 * ```
		 * @note When you load data from memory, the AssetManager copies it internally into its own MemoryDataProvider. You can
		 * can free the local memory after staging, then load and unload as much as you'd like. Unstaging frees up the AssetManager's
		 * internal store of the data.
		 * @see Ares::Systems::ThreadPool
		 * @see Ares::MemoryDataProvider
		 */
		class AssetManager : public Internal::System
		{
		public:
			/**
			 * @typedef AssetCallbackFn
			 * @brief The callback function used in the Load method.
			 * 
			 * @details The callback function used in the Load method is a void function that takes
			 * a Ref to an Asset object (the asset being loaded) as the argument.
			 */
			using AssetCallbackFn = eastl::function<void(Ref<Asset>)>;

			/**
			 * @typedef AssetListenerCallbackFn
			 * @brief The callback function used for listener callbacks.
			 * 
			 * @details The callback function used for listener callbacks is a void function that
			 * takes a reference to an Event object as the argument.
			 * @note Marking an event as "Handled" from an Asset Listener does not stop the Event
			 * from propagating through the EventQueue.
			 */
			using AssetListenerCallbackFn = eastl::function<void(Event&)>;

		private:
			using AssetId = uint32_t;
			using StoredCallbackFn = eastl::function<void()>;
		public:
			/**
			 * @brief AssetManager destructor, shuts down the system and cleans up resources.
			 */
			~AssetManager();

			/**
			 * @brief Stages an Asset for loading using a filepath.
			 * 
			 * @tparam AssetType The type of Asset to be staged.
			 * @param name The name of the Asset.
			 * @param filepath The filepath of the Asset data.
			 * @return A Ref of the staged Asset object.
			 */
			template <typename AssetType>
			inline Ref<Asset> Stage(const eastl::string& name, const eastl::string& filepath) { return Stage<AssetType>(name, filepath, {}, nullptr, 0); }

			/**
			 * @brief Stages an Asset for loading using dependencies.
			 * 
			 * @tparam AssetType The type of Asset to be staged.
			 * @param name The name of the Asset.
			 * @param dependencies A vector of dependencies required by the Asset.
			 * @return A Ref of the staged Asset object.
			 * **Example Usage**:
			 * ```cpp
			 * Ref<Asset> shaderProgramAsset = assetManager->Stage<ShaderProgram>("MyShaderProgram", {
			 *     assetManager->Stage<VertexShader>("ShaderProgram_Vertex", "path/to/vertex.glsl"),
			 *     assetManager->Stage<FragmentShader>("ShaderProgram_Fragment", "path/to/fragment.glsl")
			 * });
			 * ```
			 */
			template <typename AssetType>
			inline Ref<Asset> Stage(const eastl::string& name, const eastl::vector<Ref<Asset>>& dependencies) { return Stage<AssetType>(name, "", dependencies, nullptr, 0); }

			/**
			 * @brief Stages an Asset for loading using raw data.
			 * 
			 * @tparam AssetType The type of Asset to be staged.
			 * @param name The name of the Asset.
			 * @param data A pointer to the Asset's raw data in memory.
			 * @param size The size of the Asset's raw data in memory.
			 * @return A Ref of the staged Asset object.
			 */
			template <typename AssetType>
			inline Ref<Asset> Stage(const eastl::string& name, const void* data, const size_t& size) { return Stage<AssetType>(name, "", {}, data, size); }

			/**
			 * @brief Removes an asset completely from the AssetManager.
			 * 
			 * @param asset The asset to unstage.
			 * @warning This will remove an Asset and make it unusable. If the Asset
			 * is still loaded, it will be unloaded and unstaged. An AssetUnloadedEvent
			 * will not be dispatched though, only an AssetUnstagedEvent.
			 */
			void Unstage(const Ref<Asset>& asset);

			/**
			 * @brief Loads an Asset.
			 * 
			 * @details Asset is loaded asynchronously if a ThreadPool instance is provided when
			 * creating the AssetManager.
			 * 
			 * @param asset The Asset to load.
			 * @param callback A callback function to execute after loading. (**Optional**)
			 * 
			 * **Example Usage**:
			 * ```cpp
			 * assetManager->Load(stagedAsset, [](const Ref<Asset>& asset)
			 * {
			 *     if (asset->GetState() == AssetState::Loaded)
			 *     {
			 *         // Do something with the Asset
			 *     }
			 * });
			 * ```
			 * **Or**:
			 * ```cpp
			 * assetManager->Load(
			 *     assetManager->Stage("MyTexture", "path/to/texture.png"),
			 *     [](const Ref<Asset>& asset) {
			 *         if (asset->GetState() == AssetState::Loaded)
			 *         {
			 *             // Do something with the Asset
			 *         }
			 * }));
			 * ```
			 */
			void Load(const Ref<Asset>& asset, AssetCallbackFn && callback = nullptr);

			/**
			 * @brief Loads multiple Assets.
			 * 
			 * @details Assets are loaded asynchronously if a ThreadPool instance is provided when
			 * creating the AssetManager.
			 * 
			 * @param assets A vector of Assets to be loaded.
			 * @param callback A callback function to execute after loading. (**Optional**)
			 * @note The callback function will be called each time an Asset is loaded.
			 */
			void Load(const eastl::vector<Ref<Asset>>& assets, AssetCallbackFn && callback = nullptr);

			/**
			 * @brief Unloads an Asset.
			 * 
			 * @details If the Asset's data was provided through a filepath, the file data will be unloaded.
			 * The raw asset itself is also destroyed. After unloading, an Asset's state is "Staged".
			 * 
			 * @param asset The Asset to be unloaded.
			 */
			void Unload(const Ref<Asset>& asset);

			/**
			 * @brief Unloads multiple Assets.
			 * 
			 * @details If the Asset's data was provided through a filepath, the file data will be unloaded.
			 * The raw asset itself is also destroyed. After unloading, an Asset's state is "Staged".
			 * 
			 * @param assets A vector of Assets to be unloaded.
			 */
			void Unload(const eastl::vector<Ref<Asset>>& assets);

			/**
			 * @brief Retrieves an Asset by its name.
			 * 
			 * @param name The name of the asset.
			 * @return A Ref of the Asset object.
			 * @warning This will return `nullptr` if no Asset is found.
			 */
			Ref<Asset> GetAsset(const eastl::string& name);

			/**
			 * @brief Retrieves an Asset by its ID.
			 * 
			 * @param assetId The ID of the asset.
			 * @return A Ref of the Asset object.
			 * @warning This will return `nullptr` if no Asset is found.
			 */
			Ref<Asset> GetAsset(const AssetId& assetId);

			/**
			 * @brief Retrieves a complete list of all Assets, either staged, loaded, or failed to load.
			 * 
			 * @return A vector of Ref's of all Asset objects.
			 */
			eastl::vector<Ref<Asset>> GetCompleteList();

			/**
			 * @brief Adds a listener to an Asset by name.
			 * 
			 * @details This method adds a listener to a specific Asset by name. The callback function will
			 * be called for all events related to that specific Asset.
			 * 
			 * @param name The name of the Asset object to be listened to.
			 * @param callback The callback function of the listener.
			 * @return The ID of the added listener.
			 * 
			 * @note AssetManager Listeners cannot mark an event as "Handled." Even if you mark the event as
			 * "Handled," the event will still propagate through the EventQueue.
			 */
			const AssetListener AddListener(const eastl::string& name, AssetListenerCallbackFn&& callback);

			/**
			 * @brief Adds a listener to all Asset events.
			 * 
			 * @details This method adds a listener to all Asset related events. The callback function will
			 * be called for any and all Asset related events.
			 * 
			 * @param callback The callback function of the listener.
			 * @return The ID of the added listener.
			 * 
			 * @note AssetManager Listeners cannot mark an event as "Handled." Even if you mark the event as
			 * "Handled," the event will still propagate though the EventQueue.
			 */
			const AssetListener AddListener(AssetListenerCallbackFn&& callback);

			/**
			 * @brief Removes an Asset event listener by its ID.
			 * 
			 * @param listenerId The ID of the listener to remove.
			 */
			void RemoveListener(AssetListener& listenerId);

			void SetEventCallback(std::function<void(Event&)> callback)
			{
				m_EventCallback = std::move(callback);
			}

			/**
			 * @brief Executes all queued callbacks.
			 */
			void OnUpdate(Timestep& ts) override;

			/**
			 * @brief Creates an instance of the AssetManager class. Called during Application construction.
			 * 
			 * @details This static method creates an instance of the AssetManager class.
			 * 
			 * @param threadPool A pointer to a ThreadPool instance. If you pass `nullptr` as the argument,
			 * the AssetManager will load Assets on the **main thread**.
			 * @return A Scope of the created AssetManager object.
			 */
			static Scope<AssetManager> Create(Systems::ThreadPool* threadPool);

		private:
			/**
			 * @brief The constructor for the AssetManager.
			 *
			 * @param threadPool A pointer to a ThreadPool instance. If you pass `nullptr` as the argument,
			 * the AssetManager will load Assets on the **main thread**.
			 */
			AssetManager(Systems::ThreadPool* threadPool);

			/**
			 * @brief Internal method for staging an asset for loading.
			 *
			 * @details This method stages an asset for loading. For an asset to be staged, you must
			 * provide a name and either a filepath, a vector of dependencies, or raw data in the form
			 * of a pointer and data size.
			 *
			 * **Required Arguments**:
			 * @li **Name**
			 *
			 * and **either**:
			 * @li **Filepath**
			 * @li **Dependencies**
			 * @li **Data Pointer** and **Data Size**
			 *
			 * @tparam AssetType The type of asset to be staged.
			 * @param name The name of the asset to be staged.
			 * @param filepath The filepath of the asset's data.
			 * @param dependencies A vector of Ref<Asset> dependencies required by the asset.
			 * @param data Raw data in the form of a void pointer for the asset.
			 * @param dataSize The size of the raw data for the asset.
			 * @return A Ref to the Asset that was staged.
			 * @note If you provide the same Asset metadata, for example, the same filepath: the
			 * AssetManager will ignore the name you provided and will return the Asset that already
			 * exists with the same metadata.
			 */
			template <typename AssetType>
			Ref<Asset> Stage(const eastl::string& name, const eastl::string& filepath, const eastl::vector<Ref<Asset>>& dependencies, const void* data, const size_t& dataSize);

			/**
			 * @brief Internal method for dispatching Asset related events.
			 * 
			 * @tparam AssetEventType The type of event to be dispatched.
			 * @param asset The asset related to the event.
			 * @param message An optional message to attach to the event.
			 */
			template <typename AssetEventType>
			void DispatchAssetEvent(const Ref<Asset>& asset, const eastl::string& message = "");

			/**
			 * @brief Internal method for queuing callback functions
			 * 
			 * @param callback The callback function to queue.
			 */
			void QueueCallback(StoredCallbackFn&& callback);

			/**
			 * @brief Internal method for loading and instantiating raw assets.
			 * 
			 * @param asset The Asset to be loaded.
			 * @param callback An optional callback function to be executed on Asset load or failure.
			 */
			void LoadRawAsset(const Ref<Asset>& asset, AssetCallbackFn&& callback);

			/**
			 * @brief Internal method for retrieving a hash based on asset metadata.
			 * 
			 * @param type The type index of the Asset.
			 * @param filepath The filepath for the Asset's data.
			 * @param dependencies A vector of dependencies in the form of Asset IDs.
			 * @param dataKey The MemoryDataKey for the Asset's data.
			 * @return A hash based on all the provided data.
			 */
			const size_t GetHash(const std::type_index& type, const eastl::string& filepath, const eastl::vector<AssetId>& dependencies, const MemoryDataKey& dataKey);
			
			/**
			 * @brief Internal method for retrieving an already existing Asset.
			 * 
			 * @param contentHash The hash based on metadata to find an existing Asset with the same metadata.
			 */
			Ref<Asset> FindExistingAsset(const size_t& contentHash);

		private:
			// Asset Cache
			eastl::hash_map<AssetId, Ref<Asset>> m_AssetCache;
			eastl::atomic<AssetId> m_NextAssetId;
			std::shared_mutex m_CacheMutex;

			// Secondary Lookup Maps
			eastl::hash_map<eastl::string, AssetId> m_NameIdMap;
			eastl::hash_map<size_t, AssetId> m_HashIdMap;
			std::shared_mutex m_MapMutex;

			// Callback Queues
			eastl::queue<StoredCallbackFn> m_ReadCallbackQueue;
			eastl::queue<StoredCallbackFn> m_WriteCallbackQueue;
			std::shared_mutex m_ReadCallbackMutex;
			std::shared_mutex m_WriteCallbackMutex;

			// Asset Listeners
			eastl::hash_map<AssetListener, AssetListenerCallbackFn> m_Listeners;
			eastl::vector<AssetListener> m_ListenerOrder;
			eastl::hash_map<AssetListener, eastl::string> m_ListenerNameMap;
			eastl::hash_set<AssetListener> m_GlobalListeners;
			eastl::atomic<AssetListener> m_NextListenerId;
			std::shared_mutex m_ListenerMutex;

			// Task Queue
			eastl::queue<eastl::function<void()>> m_ReadTaskQueue;
			eastl::queue<eastl::function<void()>> m_WriteTaskQueue;
			std::shared_mutex m_ReadTaskQueueMutex;
			std::shared_mutex m_WriteTaskQueueMutex;

			// AssetManager Systems
			Systems::ThreadPool* m_ThreadPool;
			Internal::MemoryDataProviderSys m_MemoryDataProvider;

			// Event callback
			std::function<void(Event&)> m_EventCallback;
		};

		template <typename AssetType>
		Ref<Asset> AssetManager::Stage(const eastl::string& name, const eastl::string& filepath, const eastl::vector<Ref<Asset>>& dependencies, const void* data, const size_t& dataSize)
		{
			if (filepath.empty() && dependencies.size() == 0 && (data == nullptr || dataSize == 0))
			{
				AR_CORE_ASSERT(false, "Asset needs either filepath, dependencies or raw data to be staged!");
				throw std::invalid_argument("Asset needs either filepath, dependencies or raw data to be staged!");
			}

			// If data is provided, store in MemoryDataProvider
			MemoryDataKey dataKey = 0;
			if (data && dataSize > 0)
			{
				dataKey = m_MemoryDataProvider.RegisterData(data, dataSize);
			}

			// Convert dependencies into IDs
			eastl::vector<AssetId> dependencyIds;
			dependencyIds.reserve(dependencies.size());
			for (const Ref<Asset>& asset : dependencies)
			{
				dependencyIds.push_back(asset->GetAssetId());
			}

			// Compute content hash and check cache
			Ref<Asset> asset = nullptr;
			const size_t contentHash = GetHash(typeid(AssetType), filepath, dependencyIds, dataKey);
			asset = FindExistingAsset(contentHash);
			if (asset)
				return asset;

			// Ensure unique asset name
			eastl::string assetName = name;
			{
				std::unique_lock lock(m_MapMutex);
				while (m_NameIdMap[assetName] != 0)
					assetName = Utility::IncrementStringSuffix(assetName.c_str()).c_str();
			}

			// Create and initialize the asset
			const AssetId currentId = m_NextAssetId++;
			asset = Asset::Create(typeid(AssetType), AssetState::Staged, filepath, dependencyIds, dataKey, this);
			asset->SetName(assetName);
			asset->SetAssetId(currentId);

			// Update secondary lookup maps
			{
				std::unique_lock lock(m_MapMutex);
				m_NameIdMap[assetName] = currentId;
				m_HashIdMap[contentHash] = currentId;
			}

			// Add asset to cache
			{
				std::unique_lock lock(m_CacheMutex);
				m_AssetCache[currentId] = asset;
			}

			// Dispatch Event, notify listeners, and return
			DispatchAssetEvent<AssetStagedEvent>(asset);
			return asset;
		}

		template <typename AssetEventType>
		void AssetManager::DispatchAssetEvent(const Ref<Asset>& asset, const eastl::string& message)
		{
			// Make sure AssetEventType is an asset event
		#if AR_BUILD_DEBUG || AR_BUILD_RELEASE
			if (!std::is_base_of_v<AssetBaseEvent, AssetEventType>)
			{
				eastl::string errorMsg("Tried to dispatch non-asset event: " + eastl::string(typeid(AssetEventType).name()));
				AR_CORE_ASSERT(false, errorMsg.c_str());
				throw std::invalid_argument(errorMsg.c_str());
			}
		#endif

			// Create event
			AssetEventType tempEvent(asset, message.c_str());

			if (!(tempEvent.GetCategoryFlags() & EventCategoryAsset))
			{
				const char* errorMessage("Tried to dispatch an event that doesn't belong to EventCategoryAsset!");
				AR_CORE_ASSERT(false, errorMessage);
				throw std::invalid_argument(errorMessage);
			}

			// Dispatch to event queue
			//EventQueue::Dispatch<AssetEventType>(tempEvent);
			if (m_EventCallback)
				m_EventCallback(tempEvent);
			Ref<AssetEventType> event = CreateRef<AssetEventType>(tempEvent);

			// Dispatch to asset listeners
			{
				std::shared_lock lock(m_ListenerMutex);
				for (AssetListener& listener : m_ListenerOrder)
				{
					// Get listener callback
					auto listenerFuncIt = m_Listeners.find(listener);
					if (listenerFuncIt != m_Listeners.end() && listenerFuncIt->second != nullptr)
					{
						// Check to see if listener is global
						if (m_GlobalListeners.find(listener) != m_GlobalListeners.end())
						{
							QueueCallback([func = listenerFuncIt->second, event]() { func(*event); });
							return;
						}

						// Check to see if listener is specific
						auto specificListenerIt = m_ListenerNameMap.find(listener);
						if (specificListenerIt != m_ListenerNameMap.end() && specificListenerIt->second == asset->GetName())
						{
							QueueCallback([func = listenerFuncIt->second, event]() { func(*event); });
							return;
						}
					}
				}
			}
		}

	}

}