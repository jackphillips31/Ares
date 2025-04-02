/**
 * @file AssetManager.h
 * @brief Declaration of the [AssetManager](#Ares::Systems::AssetManager)
 * class for managing assets, including staging, loading, unloading,
 * unstaging, and listener systems.
 * 
 * @details The AssetManager is a [System](#Ares::Internal::System) sub-class responsible for handling all asset-related
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
#include "Engine/Containers/Atomic.h"
#include "Engine/Containers/Function.h"
#include "Engine/Containers/HashMap.h"
#include "Engine/Containers/HashSet.h"
#include "Engine/Containers/InitializerList.h"
#include "Engine/Containers/Queue.h"
#include "Engine/Containers/String.h"
#include "Engine/Containers/StringView.h"
#include "Engine/Containers/Vector.h"
#include "Engine/Core/System.h"

namespace Ares {

	class Application;
	class Asset;
	class AssetStagedEvent;
	class Event;
	class Timestep;
	struct RawData;

	namespace Internal {

		class MemoryDataProvider;

	}

	/**
	 * @typedef AssetListener
	 * @brief Listener ID stored as a 32 bit unsigned integer.
	 */
	using AssetListener = uint32_t;
	using MemoryDataKey = uint32_t;

	namespace Systems {

		class MainThreadQueue;
		class ThreadPool;

		/**
		 * @class AssetManager
		 * @brief Manages the loading, unloading, and staging of [Assets](#Ares::Asset).
		 *
		 * @details The AssetManager handles asset lifecycle operations, including staging, loading, and
		 * unloading.
		 * [Assets](#Ares::Asset) can be loaded from files or from memory, and loading can be performed on
		 * a separate thread if a ThreadPool is provided. If no ThreadPool is provided, loading occurs on
		 * the main thread.
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
		private:
			/**
			 * @brief The constructor for the AssetManager.
			 *
			 * @param threadPool A pointer to a ThreadPool instance. If you pass `nullptr` as the argument,
			 * the AssetManager will load [Assets](#Ares::Asset) on the **main thread**.
			 */
			AssetManager(Systems::ThreadPool* threadPool);

			/**
			 * @typedef AssetId
			 * @brief Asset ID stored as a uint32_t.
			 */
			using AssetId = uint32_t;

			/**
			 * @typedef StoredCallbackFn
			 * @brief Stored void function used for callbacks.
			 */
			using StoredCallbackFn = Function<void()>;

		public:
			/**
			 * @brief Creates an instance of the AssetManager class. Called during Application construction.
			 *
			 * @details This static method creates an instance of the AssetManager class.
			 *
			 * @param threadPool A pointer to a ThreadPool instance. If you pass `nullptr` as the argument,
			 * the AssetManager will load Assets on the **main thread**.
			 * @return A Scope of the created AssetManager object.
			 */
			static Scope<AssetManager> Create(Systems::ThreadPool* threadPool = nullptr);

			/**
			 * @brief Destroys the AssetManager and cleans up resources.
			 */
			~AssetManager() override;

			/**
			 * @typedef AssetCallbackFn
			 * @brief The callback function used in the [Load](#Ares::Systems::AssetManager::Load) method.
			 *
			 * @details The callback function used in the [Load](#Ares::Systems::AssetManager::Load)
			 * method is a void function that takes a Ref to an Asset object (the asset being loaded)
			 * as the argument.
			 */
			using AssetCallbackFn = Function<void(Ref<Asset>)>;

			/**
			 * @typedef EventCallbackFn
			 * @brief The callback function used in the [AddListener](#Ares::Systems::AssetManager::AddListener) method
			 * and the [SetEventCallback](#Ares::Systems::AssetManager::SetEventCallback) method.
			 *
			 * @details The callback function used in the [AddListener](#Ares::Systems::AssetManager::AddListener) method
			 * and the [SetEventCallback](#Ares::Systems::AssetManager::SetEventCallback) method.
			 * This function takes a reference to an Event object as the argument.
			 * @note Marking an Event as "Handled" from an AssetListener does not stop the Event
			 * from propagating through the [EventQueue](#Ares::Systems::EventQueue).
			 */
			using EventCallbackFn = Function<void(Event&)>;

			/**
			 * @brief Stages an Asset for loading using a filepath.
			 *
			 * @details After staging, an Asset's state is AssetState::Staged.
			 * @tparam AssetType The type of Asset to be staged.
			 * @param name The name of the Asset.
			 * @param filepath The filepath of the Asset data.
			 * @return A Ref of the staged Asset object.
			 *
			 * **Example Usage**:
			 * ```cpp
			 * Ref<Asset> textureAsset = assetManager->Stage<Texture>("MyTexture", "path/to/texture.png");
			 * ```
			 */
			template <typename AssetType>
			Ref<Asset> Stage(const String& name, const String& filepath);

			/**
			 * @brief Stages an Asset for loading using dependencies.
			 *
			 * @details After staging, an Asset's state is AssetState::Staged.
			 * @tparam AssetType The type of Asset to be staged.
			 * @param name The name of the Asset.
			 * @param dependencies A vector of dependencies required by the Asset.
			 * @return A Ref of the staged Asset object.
			 *
			 * **Example Usage**:
			 * ```cpp
			 * Ref<Asset> shaderProgramAsset = assetManager->Stage<ShaderProgram>("MyShaderProgram", {
			 *     assetManager->Stage<VertexShader>("ShaderProgram_Vertex", "path/to/vertex.glsl"),
			 *     assetManager->Stage<FragmentShader>("ShaderProgram_Fragment", "path/to/fragment.glsl")
			 * });
			 * ```
			 */
			template <typename AssetType>
			Ref<Asset> Stage(const String& name, const InitializerList<Ref<Asset>>& dependencies);

			/**
			 * @brief Stages an Asset for loading using raw data.
			 *
			 * @details After staging, an Asset's state is AssetState::Staged.
			 * @tparam AssetType The type of Asset to be staged.
			 * @param name The name of the Asset.
			 * @param data A pointer to the Asset's raw data in memory.
			 * @param size The size of the Asset's raw data in memory.
			 * @return A Ref of the staged Asset object.
			 *
			 * **Example Usage**:
			 * ```cpp
			 * Ref<Asset> meshAsset = assetManager->Stage<MeshData>("MyMeshData", dataPtr, dataSize);
			 * ```
			 */
			template <typename AssetType>
			Ref<Asset> Stage(const String& name, const void* data, const size_t& size);

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
			 * @details Asset is loaded asynchronously if a [ThreadPool](#Ares::Systems::ThreadPool)
			 * instance is provided when creating the [AssetManager](#Ares::Systems::AssetManager).
			 * After loading, an Asset's state is AssetState::Loaded.
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
			 * @throws std::invalid_argument If the Asset is not staged or corrupted.
			 */
			void Load(const Ref<Asset>& asset, AssetCallbackFn&& callback = nullptr);

			/**
			 * @brief Loads multiple Assets.
			 *
			 * @details Assets are loaded asynchronously if a ThreadPool instance is provided when
			 * creating the AssetManager. After loading, an Asset's state is AssetState::Loaded.
			 *
			 * @param assets A vector of [Assets](#Ares::Asset) to be loaded.
			 * @param callback A callback function to execute after loading. (**Optional**)
			 * @note The callback function will be called each time an Asset is loaded.
			 * @throws std::invalid_argument If the Asset is not staged or corrupted.
			 */
			void Load(const InitializerList<Ref<Asset>>& assets, AssetCallbackFn&& callback = nullptr);

			/**
			 * @brief Unloads an Asset.
			 *
			 * @details If the Asset's data was provided through a filepath, the file data will be unloaded.
			 * The raw asset itself is also destroyed. After unloading, an Asset's state is AssetState::Staged.
			 *
			 * @param asset The Asset to be unloaded.
			 */
			void Unload(const Ref<Asset>& asset);

			/**
			 * @brief Unloads multiple Assets.
			 *
			 * @details If the Asset's data was provided through a filepath, the file data will be unloaded.
			 * The raw asset itself is also destroyed. After unloading, an Asset's state is AssetState::Staged.
			 *
			 * @param assets A vector of Assets to be unloaded.
			 */
			void Unload(const InitializerList<Ref<Asset>>& assets);

			/**
			 * @brief Retrieves an Asset by its name.
			 *
			 * @param name The name of the asset.
			 * @return A Ref of the Asset object.
			 * @warning This will return `nullptr` if no Asset is found.
			 */
			Ref<Asset> GetAsset(const String& name);

			/**
			 * @brief Retrieves an Asset by its ID.
			 *
			 * @param assetId The ID of the asset.
			 * @return A Ref of the Asset object.
			 * @warning This will return `nullptr` if no Asset is found.
			 */
			Ref<Asset> GetAsset(const AssetId& assetId);

			/**
			 * @brief Retrieves a complete list of all [Assets](#Ares::Asset), either staged, loaded, or failed to load.
			 *
			 * @param vec The vector to be filled with the results.
			 */
			void GetCompleteList(Vector<Ref<Asset>>& vec);

			/**
			 * @brief Adds a listener to an Asset by name.
			 *
			 * @details This method adds a listener to a specific Asset by name. The callback function will
			 * be called for all events related to that specific Asset.
			 *
			 * @param name The name of the Asset object to be listened to.
			 * @param callback The [callback function](#Ares::Systems::AssetManager::EventCallbackFn) of the listener.
			 * @return The [Listener ID](#Ares::AssetListener) of the added listener.
			 *
			 * @note AssetManager Listeners cannot mark an Event as "Handled." Even if you mark the Event as
			 * "Handled," the Event will still propagate through the EventQueue.
			 */
			const AssetListener AddListener(const String& name, EventCallbackFn&& callback);

			/**
			 * @brief Adds a listener to all Asset events.
			 *
			 * @details This method adds a listener to all Asset related events. The callback function will
			 * be called for any and all Asset related events.
			 *
			 * @param callback The [callback function](#Ares::Systems::AssetManager::EventCallbackFn) of the listener.
			 * @return The [Listener ID](#Ares::AssetListener) of the added listener.
			 *
			 * @note AssetManager Listeners cannot mark an Event as "Handled." Even if you mark the Event as
			 * "Handled," the Event will still propagate though the EventQueue.
			 */
			const AssetListener AddListener(EventCallbackFn&& callback);

			/**
			 * @brief Removes an Asset event listener by its [Listener ID](#Ares::AssetListener).
			 *
			 * @param listenerId The [ID](#Ares::AssetListener) of the listener to remove.
			 */
			void RemoveListener(AssetListener& listenerId);

			/**
			 * @brief Removes multiple Asset event listeners by their [Listener ID](#Ares::AssetListener).
			 * 
			 * @param listenerIds The [IDs](#Ares::AssetListener) of the listeners to remove.
			 */
			void RemoveListener(InitializerList<AssetListener>& ids);

			/**
			 * @brief Sets the event callback function for the AssetManager.
			 *
			 * @details This method allows you to set a callback function that the
			 * AssetManager will use to dispatch events.
			 * The [callback function](#Ares::Systems::AssetManager::EventCallbackFn) must accept
			 * a single parameter of the Event type and return `void`.
			 * Typically, this is used to forward events to an [EventQueue::Dispatch](#Ares::Systems::EventQueue::Dispatch)
			 * method, enabling centralized event handling.
			 *
			 * @param callback An rvalue reference to an function object representing the callback.
			 * The [callback signature](#Ares::Systems::AssetManager::EventCallbackFn) must be `void(Event&)`.
			 *
			 * **Example Usage**:
			 * ```cpp
			 * using namespace Ares;
			 *
			 * Application& app = Application::Get();
			 * app.GetSystem<Systems::AssetManager>()->SetEventCallback(
			 *     AR_BIND_SYSTEM_CALLBACK_FN(Systems::EventQueue::Dispatch, app.GetSystem<Systems::EventQueue>())
			 * );
			 * ```
			 *
			 * @note The provided callback is moved into the AssetManager ensuring efficient transfer of ownership.
			 * The callback will be invoked whenever the AssetManager needs to dispatch an event. Ensure the callback
			 * is valid and properly handles the event to avoid undefined behavior.
			 * @see EventQueue::Dispatch
			 */
			void SetEventCallback(EventCallbackFn&& callback);

			/**
			 * @brief Executes all queued callbacks.
			 *
			 * @details This OnUpdate method swaps the queues and executes callbacks.
			 *
			 * @param ts The Timestep representing the time elapsed since the last update.
			 */
			void OnUpdate(const Timestep& ts) override;

		private:
			template <typename ObjectType, typename... Args>
			friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

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
			Ref<Asset> Stage(const String& name, const String& filepath, const InitializerList<Ref<Asset>>& dependencies, const void* data, const size_t& dataSize);

			/**
			 * @brief Internal method for loading and instantiating raw assets.
			 *
			 * @param asset The Asset to be loaded.
			 * @param callback An optional callback function to be executed on Asset load or failure.
			 */
			void LoadRawAsset(const Ref<Asset>& asset, AssetCallbackFn&& callback);

			/**
			 * @brief Internal method for queuing callback functions
			 *
			 * @param callback The callback function to queue.
			 */
			void QueueCallback(StoredCallbackFn&& callback);

			/**
			 * @brief Internal method for retrieving a hash based on asset metadata.
			 *
			 * @param type The type index of the Asset.
			 * @param filepath The filepath for the Asset's data.
			 * @param dependencies A vector of dependencies in the form of Asset IDs.
			 * @param dataKey The MemoryDataKey for the Asset's data.
			 * @return A hash based on all the provided data.
			 */
			const size_t GetHash(const std::type_index& type, const String& filepath, const Vector<AssetId>& dependencies, const MemoryDataKey& dataKey);

			/**
			 * @brief Internal method for retrieving an already existing Asset.
			 *
			 * @param contentHash The hash based on metadata to find an existing Asset with the same metadata.
			 */
			Ref<Asset> FindExistingAsset(const size_t& contentHash);

			/**
			 * @brief Internal method for dispatching Asset related events.
			 *
			 * @tparam AssetEventType The type of event to be dispatched.
			 * @param asset The asset related to the event.
			 * @param message An optional message to attach to the event.
			 */
			template <typename AssetEventType>
			void DispatchAssetEvent(const Ref<Asset>& asset, const char* message = "");

		private:
			// Asset Cache
			HashMap<AssetId, Ref<Asset>, true> m_AssetCache;
			Atomic<AssetId> m_NextAssetId;
			std::shared_mutex m_CacheMutex;

			HashMap<AssetId, Vector<AssetId>, true> m_DependencyCache;
			std::shared_mutex m_DependencyMutex;

			// Secondary Lookup Maps
			HashMap<String, AssetId, true> m_NameIdMap;
			HashMap<size_t, AssetId, true> m_HashIdMap;
			std::shared_mutex m_MapMutex;

			// Callback Queues
			Queue<StoredCallbackFn> m_ReadCallbackQueue;
			Queue<StoredCallbackFn> m_WriteCallbackQueue;
			std::shared_mutex m_ReadCallbackMutex;
			std::shared_mutex m_WriteCallbackMutex;

			// Asset Listeners
			HashMap<AssetListener, EventCallbackFn, true> m_Listeners;
			Vector<AssetListener> m_ListenerOrder;
			HashMap<AssetListener, String, true> m_ListenerNameMap;
			HashSet<AssetListener, true> m_GlobalListeners;
			Atomic<AssetListener> m_NextListenerId;
			std::shared_mutex m_ListenerMutex;

			// Task Queue
			Queue<Function<void()>> m_ReadTaskQueue;
			Queue<Function<void()>> m_WriteTaskQueue;
			std::shared_mutex m_ReadTaskQueueMutex;
			std::shared_mutex m_WriteTaskQueueMutex;

			// AssetManager Systems
			Systems::MainThreadQueue* m_MainThreadQueue;
			Systems::ThreadPool* m_ThreadPool;
			Scope<Internal::MemoryDataProvider> m_MemoryDataProvider;

			// Event callback
			Function<void(Event&)> m_EventCallback;
		};

	}

}