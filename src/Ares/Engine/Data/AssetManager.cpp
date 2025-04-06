#include <arespch.h>
#include "Engine/Data/AssetManager.h"

#include "Engine/Containers/Pair.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/MainThreadQueue.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Data/Asset.h"
#include "Engine/Data/DataBuffer.h"
#include "Engine/Data/FileIO.h"
#include "Engine/Data/MemoryDataProvider.h"
#include "Engine/Data/RawData.h"
#include "Engine/Data/Parsers/OBJParser.h"
#include "Engine/Data/Parsers/ShaderParser.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Renderer/Assets/MeshData.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Assets/Texture.h"
#include "Engine/Utility/Hash.h"
#include "Engine/Utility/String.h"
#include "Engine/Utility/Type.h"

namespace Ares::Systems {

	template Ref<Asset> AssetManager::Stage<ShaderProgram>(StringView, StringView, const std::initializer_list<Ref<Asset>>&, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<VertexShader>(StringView, StringView, const std::initializer_list<Ref<Asset>>&, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<FragmentShader>(StringView, StringView, const std::initializer_list<Ref<Asset>>&, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<MeshData>(StringView, StringView, const std::initializer_list<Ref<Asset>>&, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<Texture>(StringView, StringView, const std::initializer_list<Ref<Asset>>&, const void*, const size_t&);

	template Ref<Asset> AssetManager::Stage<ShaderProgram>(StringView, StringView);
	template Ref<Asset> AssetManager::Stage<VertexShader>(StringView, StringView);
	template Ref<Asset> AssetManager::Stage<FragmentShader>(StringView, StringView);
	template Ref<Asset> AssetManager::Stage<MeshData>(StringView, StringView);
	template Ref<Asset> AssetManager::Stage<Texture>(StringView, StringView);

	template Ref<Asset> AssetManager::Stage<ShaderProgram>(StringView, const std::initializer_list<Ref<Asset>>&);
	template Ref<Asset> AssetManager::Stage<VertexShader>(StringView, const std::initializer_list<Ref<Asset>>&);
	template Ref<Asset> AssetManager::Stage<FragmentShader>(StringView, const std::initializer_list<Ref<Asset>>&);
	template Ref<Asset> AssetManager::Stage<MeshData>(StringView, const std::initializer_list<Ref<Asset>>&);
	template Ref<Asset> AssetManager::Stage<Texture>(StringView, const std::initializer_list<Ref<Asset>>&);

	template Ref<Asset> AssetManager::Stage<ShaderProgram>(StringView, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<VertexShader>(StringView, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<FragmentShader>(StringView, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<MeshData>(StringView, const void*, const size_t&);
	template Ref<Asset> AssetManager::Stage<Texture>(StringView, const void*, const size_t&);

	template void AssetManager::DispatchAssetEvent<AssetStagedEvent>(const Ref<Asset>&, StringView);
	template void AssetManager::DispatchAssetEvent<AssetLoadedEvent>(const Ref<Asset>&, StringView);
	template void AssetManager::DispatchAssetEvent<AssetUnloadedEvent>(const Ref<Asset>&, StringView);
	template void AssetManager::DispatchAssetEvent<AssetUnstagedEvent>(const Ref<Asset>&, StringView);
	template void AssetManager::DispatchAssetEvent<AssetFailedEvent>(const Ref<Asset>&, StringView);

	Scope<AssetManager> AssetManager::Create(Systems::ThreadPool* threadPool, Systems::MainThreadQueue* mainThreadQueue)
	{
		return CreateScope<AssetManager>(threadPool, mainThreadQueue);
	}

	AssetManager::AssetManager(Systems::ThreadPool* threadPool, Systems::MainThreadQueue* mainThreadQueue)
		: m_NextAssetId(1), m_NextListenerId(1), m_ThreadPool(threadPool), m_MainThreadQueue(mainThreadQueue), m_MemoryDataProvider(CreateScope<Internal::MemoryDataProvider>())
	{
		AR_CORE_INFO("Initializing System: AssetManager");

	#if AR_BUILD_DEBUG
		if (!m_ThreadPool)
		{
			AR_CORE_WARN("AssetManager: ThreadPool not provided, loading will execute on the main thread!");
		}
		else if (!m_MainThreadQueue)
		{
			AR_CORE_WARN("AssetManager: Failed to retrieve MainThreadQueue. This is necessary when running on a ThreadPool!");
		}
	#endif

		m_ListenerOrder.reserve(100);
	}

	AssetManager::~AssetManager()
	{
		{
			std::unique_lock lock(m_CacheMutex);
			m_AssetCache.clear();
		}
		{
			std::unique_lock lock(m_MapMutex);
			m_NameIdMap.clear();
			m_HashIdMap.clear();
		}
		{
			std::unique_lock lock1(m_ReadCallbackMutex, std::defer_lock);
			std::unique_lock lock2(m_WriteCallbackMutex, std::defer_lock);
			std::lock(lock1, lock2);
			while (!m_ReadCallbackQueue.empty()) m_ReadCallbackQueue.pop();
			while (!m_WriteCallbackQueue.empty()) m_WriteCallbackQueue.pop();
		}
		{
			std::unique_lock lock(m_ListenerMutex);
			m_Listeners.clear();
			m_GlobalListeners.clear();
			m_ListenerOrder.clear();
			m_ListenerNameMap.clear();
		}
	}

	template <typename AssetType>
	Ref<Asset> AssetManager::Stage(StringView name, StringView filepath)
	{
		return Stage<AssetType>(name, filepath, {}, nullptr, 0);
	}

	template <typename AssetType>
	Ref<Asset> AssetManager::Stage(StringView name, const std::initializer_list<Ref<Asset>>& dependencies)
	{
		return Stage<AssetType>(name, nullptr, dependencies, nullptr, 0);
	}

	template <typename AssetType>
	Ref<Asset> AssetManager::Stage(StringView name, const void* data, const size_t& size)
	{
		return Stage<AssetType>(name, nullptr, {}, data, size);
	}

	template <typename AssetType>
	Ref<Asset> AssetManager::Stage(StringView name, StringView filepath, const std::initializer_list<Ref<Asset>>& dependencies, const void* data, const size_t& dataSize)
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
			dataKey = m_MemoryDataProvider->RegisterData(data, dataSize);
		}

		const AssetId currentId = m_NextAssetId++;

		// Convert dependencies into IDs
		std::unique_lock lock(m_DependencyMutex);
		Vector<AssetId>& dependencyIds = m_DependencyCache[currentId];
		dependencyIds.reserve(dependencies.size());
		for (const Ref<Asset>& asset : dependencies)
		{
			dependencyIds.emplace_back(asset->GetAssetId());
		}
		lock.unlock();

		// Compute content hash and check cache
		Ref<Asset> asset = nullptr;
		const size_t contentHash = GetHash(typeid(AssetType), filepath, dependencyIds, dataKey);
		asset = FindExistingAsset(contentHash);
		if (asset)
			return asset;

		// Ensure unique asset name
		String assetName(name);
		{
			std::unique_lock lock(m_MapMutex);
			while (m_NameIdMap.find(assetName) != m_NameIdMap.end())
				Utility::IncrementStringSuffix(assetName);
		}

		// Create and initialize the asset
		m_NextAssetId++;
		asset = Asset::Create(typeid(AssetType), AssetState::Staged, filepath, dependencyIds, dataKey, this);
		asset->SetName(assetName);
		asset->SetAssetId(currentId);

		// Update secondary lookup maps
		{
			std::unique_lock lock(m_MapMutex);
			m_NameIdMap.try_emplace(eastl::move(assetName), currentId);
			m_HashIdMap.try_emplace(eastl::move(contentHash), currentId);
		}

		// Add asset to cache
		{
			std::unique_lock lock(m_CacheMutex);
			m_AssetCache.try_emplace(eastl::move(currentId), asset);
		}

		// Dispatch Event, notify listeners, and return
		DispatchAssetEvent<AssetStagedEvent>(asset);
		return asset;
	}

	void AssetManager::Unstage(const Ref<Asset>& asset)
	{
	#if AR_BUILD_DEBUG
		// Check for references outside AssetManager
		if (asset.use_count() > 1)
		{
			AR_CORE_WARN("Asset: {} has {} reference(s) outside the Asset Cache!", asset->GetName(), asset.use_count() - 1);
		}
	#endif

		// Check to see if asset has been unloaded first
		if (asset->GetState() == AssetState::Loaded)
		{
			AR_CORE_WARN("AssetManager: {} has not been unloaded! Current State: {} - Asset will be removed regardless...", asset->GetName(), asset->GetStateString());
			Unload(asset);
		}

		// Remove from secondary lookup maps
		{
			std::unique_lock lock(m_MapMutex);
			m_NameIdMap.erase(asset->GetName());
			m_HashIdMap.erase(eastl::hash<Asset>()(asset));
		}

		// Remove from asset cache
		{
			std::unique_lock lock(m_CacheMutex);
			m_AssetCache.erase(asset->GetAssetId());
		}

		// Remove data from memory provider
		if (asset->GetDataKey())
		{
			m_MemoryDataProvider->UnregisterData(asset->GetDataKey());
		}

		asset->Unstage();

		DispatchAssetEvent<AssetUnstagedEvent>(asset);
	}

	void AssetManager::Load(const Ref<Asset>& asset, AssetCallbackFn&& callback)
	{
		if (!asset)
		{
			AR_CORE_ASSERT(false, "AssetManager: Asset is either corrupted or not staged!");
			throw std::invalid_argument("AssetManager: Asset is either corrupted or not staged!");
		}

		const AssetState& state = asset->GetState();
		if (state != AssetState::Staged)
		{
			if (state == AssetState::Failed)
			{
				AR_CORE_ERROR("AssetManager: Attempted to load failed asset - {}", asset->GetName());
			}
			if (callback) callback(asset);
			return;
		}

		std::shared_lock lock(m_DependencyMutex);
		const Vector<AssetId>& dependencies = m_DependencyCache[asset->GetAssetId()];
		if (!dependencies.empty())
		{
			for (const AssetId& currentId : dependencies)
			{
				if (Ref<Asset> currentDep = GetAsset(currentId))
				{
					if (currentDep->GetState() == AssetState::Staged)
					{
						LoadRawAsset(currentDep, [this, asset, callback = eastl::move(callback)](Ref<Asset>) mutable
						{
								Load(asset, eastl::move(callback));
						});
						return;
					}
					else if (currentDep->GetState() == AssetState::Failed)
					{
						AR_CORE_ERROR("AssetManager: Asset dependency - {} - failed to load!");
					}
				}
			}
		}

		LoadRawAsset(asset, eastl::move(callback));
	}

	void AssetManager::Load(const std::initializer_list<Ref<Asset>>& assets, AssetCallbackFn&& callback)
	{
		for (const Ref<Asset>& asset : assets)
		{
			AssetCallbackFn callbackCopy = callback;
			Load(asset, eastl::move(callbackCopy));
		}
	}

	void AssetManager::Unload(const Ref<Asset>& asset)
	{
		// Unload asset
		if (asset->HasFilepath() && asset->GetDataKey())
		{
			m_MemoryDataProvider->UnregisterData(asset->GetDataKey());
			asset->SetDataKey(0);
		}
		asset->Unload();
		DispatchAssetEvent<AssetUnloadedEvent>(asset);
	}

	void AssetManager::Unload(const std::initializer_list<Ref<Asset>>& assets)
	{
		for (const Ref<Asset>& asset : assets)
			Unload(asset);
	}

	Ref<Asset> AssetManager::GetAsset(StringView name)
	{
		AssetId cacheId = 0;
		{
			std::shared_lock lock(m_MapMutex);
			if (const auto& it = m_NameIdMap.find(name); it != m_NameIdMap.end())
			{
				cacheId = it->second;
				lock.unlock();
				return GetAsset(cacheId);
			}
		}
		return nullptr;
	}

	Ref<Asset> AssetManager::GetAsset(const AssetId& assetId)
	{
		std::shared_lock lock(m_CacheMutex);
		if (const auto& it = m_AssetCache.find(assetId); it != m_AssetCache.end())
			return it->second;
		else
			return nullptr;
	}

	void AssetManager::GetCompleteList(Vector<Ref<Asset>>& vec)
	{
		std::shared_lock lock(m_CacheMutex);

 		if (vec.capacity() < m_AssetCache.size())
		{
			vec.reserve(m_AssetCache.size());
		}

		vec.clear();

		for (const auto& entry : m_AssetCache)
		{
			vec.emplace_back(entry.second);
		}
	}

	const AssetListener AssetManager::AddListener(StringView name, EventCallbackFn&& callback)
	{
		std::unique_lock lock(m_ListenerMutex);
		AssetListener currentId = m_NextListenerId++;
		m_Listeners[currentId] = eastl::move(callback);
		m_Listeners.try_emplace(currentId, eastl::move(callback));
		m_ListenerNameMap.try_emplace(currentId, name);
		m_ListenerOrder.push_back(currentId);
		return currentId;
	}

	const AssetListener AssetManager::AddListener(EventCallbackFn&& callback)
	{
		std::unique_lock lock(m_ListenerMutex);
		AssetListener currentId = m_NextListenerId++;
		m_Listeners[currentId] = eastl::move(callback);
		m_Listeners.try_emplace(currentId, eastl::move(callback));
		m_GlobalListeners.emplace(currentId);
		m_ListenerOrder.emplace_back(currentId);
		return currentId;
	}

	void AssetManager::RemoveListener(AssetListener& listenerId)
	{
		if (!listenerId)
		{
			AR_CORE_WARN("AssetManager: Asset Listener has already been removed!");
			return;
		}

		std::unique_lock lock(m_ListenerMutex);

		if (const auto& it = m_Listeners.find(listenerId); it != m_Listeners.end())
		{
			m_GlobalListeners.erase(listenerId);
			m_ListenerNameMap.erase(listenerId);
			m_Listeners.erase(it);

			listenerId = 0;
		}
		else
		{
			AR_CORE_WARN("AssetManager: Did not find any asset listeners with id - {}", listenerId);
		}
	}

	void AssetManager::RemoveListener(InitializerList<AssetListener>& ids)
	{
		for (auto id : ids)
		{
			RemoveListener(id);
		}
	}

	void AssetManager::SetEventCallback(EventCallbackFn&& callback)
	{
		m_EventCallback = eastl::move(callback);
	}

	void AssetManager::OnUpdate(const Timestep& ts)
	{
		{
			std::unique_lock lock1(m_ReadTaskQueueMutex);
			std::unique_lock lock2(m_WriteTaskQueueMutex);
			eastl::swap(m_ReadTaskQueue, m_WriteTaskQueue);
		}

		{
			std::unique_lock lock(m_ReadTaskQueueMutex);
			while (!m_ReadTaskQueue.empty())
			{
				eastl::function<void()> task = eastl::move(m_ReadTaskQueue.front());
				m_ReadTaskQueue.pop();
				task();
			}
		}

		{
			std::unique_lock lock1(m_ReadCallbackMutex);
			std::unique_lock lock2(m_WriteCallbackMutex);
			eastl::swap(m_ReadCallbackQueue, m_WriteCallbackQueue);
		}

		while (!m_ReadCallbackQueue.empty())
		{
			Function<void()> callback = eastl::move(m_ReadCallbackQueue.front());
			m_ReadCallbackQueue.pop();
			callback();
		}
	}

	void AssetManager::LoadRawAsset(const Ref<Asset>& asset, AssetCallbackFn&& callback)
	{
		// Mark asset's state as "Loading"
		asset->SetState(AssetState::Loading);

		// Dispatch AssetLoadingEvent
		DispatchAssetEvent<AssetLoadingEvent>(asset);

		// Create loading task
		auto loadTask = [this, asset, callback = eastl::move(callback)]() mutable
		{
			String eventMessage;

			// Retrieve the asset type ID to determine how to process the asset
			const Utility::AssetType assetType = Utility::GetAssetType(asset->GetType());
			try
			{
				// Check if asset is valid
				if (asset->GetDependencies().size() == 0 && !asset->HasFilepath() && !asset->GetDataKey())
				{
					String errorMsg;
					fmt::format_to(eastl::back_inserter(errorMsg), "Staged [{}] {} asset doesn't have valid metadata! (no dependencies, filepath, or MemoryDataKey!", asset->GetName(), asset->GetTypeName());
					throw std::runtime_error(errorMsg.c_str());
				}

				// Load file (if provided) into MemoryDataProvider
				if (asset->HasFilepath())
				{
					DataBuffer fileData = FileIO::LoadFile(asset->GetFilepath());
					MemoryDataKey fileKey = m_MemoryDataProvider->RegisterData(eastl::move(fileData));
					if (!fileKey)
					{
						throw std::runtime_error("Failed to register data with the MemoryDataProvider!");
					}
					asset->SetDataKey(fileKey);
				}

				// Handle asset loading based on its type
				switch (assetType)
				{
				case Utility::AssetType::VertexShader:
				case Utility::AssetType::FragmentShader:
				{
					MemoryDataKey dataKey = asset->GetDataKey();
					const DataBuffer& dataBuffer = m_MemoryDataProvider->GetDataBuffer(dataKey);
					StringView shaderData(static_cast<const char*>(dataBuffer.GetBuffer()), dataBuffer.GetSize());

					std::unique_lock lock(m_WriteTaskQueueMutex);
					auto task = [this, asset, callback = eastl::move(callback), shaderData, assetType]()
						{
							Scope<Shader> result = nullptr;
							try
							{
								switch (assetType)
								{
								case Utility::AssetType::VertexShader:
								{
									result = VertexShader::Create(asset->GetName(), shaderData);
									break;
								}
								case Utility::AssetType::FragmentShader:
								{
									result = FragmentShader::Create(asset->GetName(), shaderData);
									break;
								}
								default: throw std::runtime_error("Asset Type not found!");
								}

								if (!result)
									throw std::runtime_error("Failed to create Shader asset!");

								switch (assetType)
								{
								case Utility::AssetType::VertexShader:
								{
									asset->SetAsset(eastl::move(result));
									break;
								}
								case Utility::AssetType::FragmentShader:
								{
									asset->SetAsset(eastl::move(result));
									break;
								}
								}

								asset->SetState(AssetState::Loaded);
								DispatchAssetEvent<AssetLoadedEvent>(asset);
							}
							catch (std::exception& e)
							{
								AR_CORE_ERROR("Shader Creation Error: {}", e.what());
								asset->SetState(AssetState::Failed);
								DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
							}
							if (callback)
								QueueCallback([callback = eastl::move(callback), asset]() { callback(asset); });
						};

					if (!m_MainThreadQueue)
					{
						m_WriteTaskQueue.emplace(eastl::move(task));
					}
					else
					{
						m_MainThreadQueue->SubmitTask(eastl::move(task));
					}

					break;
				}
				case Utility::AssetType::ShaderProgram:
				{
					if (asset->GetDependencies().size() > 0)
					{
						Vector<Shader*> shaders;
						shaders.reserve(asset->GetDependencies().size());
						for (const AssetId& assetId : asset->GetDependencies())
						{
							Ref<Asset> dependency = GetAsset(assetId);
							if (dependency->GetState() != AssetState::Loaded)
								throw std::runtime_error("Shader Program dependency not loaded!");

							if (dependency->GetType() == typeid(VertexShader))
							{
								shaders.push_back(dependency->GetAsset<VertexShader>());
								continue;
							}
							else if (dependency->GetType() == typeid(FragmentShader))
							{
								shaders.push_back(dependency->GetAsset<FragmentShader>());
								continue;
							}
						}

						std::unique_lock lock(m_WriteTaskQueueMutex);

						auto task = [this, asset, callback = eastl::move(callback), shaders]()
							{
								Scope<ShaderProgram> result = nullptr;
								try
								{
									result = ShaderProgram::Create(asset->GetName(), shaders);

									if (!result)
										throw std::runtime_error("Failed to create the ShaderProgram asset!");

									asset->SetAsset(eastl::move(result));
									asset->SetState(AssetState::Loaded);
									DispatchAssetEvent<AssetLoadedEvent>(asset);
								}
								catch (std::exception& e)
								{
									AR_CORE_ERROR("Shader Program Creation Error: {}", e.what());
									asset->SetState(AssetState::Failed);
									DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
								}
								if (callback)
									QueueCallback([callback = eastl::move(callback), asset]() { callback(asset); });
							};

						if (!m_MainThreadQueue)
						{
							m_WriteTaskQueue.emplace(eastl::move(task));
						}
						else
						{
							m_MainThreadQueue->SubmitTask(eastl::move(task));
						}
					}
					else
					{
						Ref<ParsedShaderData> shaderData = CreateRef<ParsedShaderData>(
							ShaderParser::ParseShaders(m_MemoryDataProvider->GetDataBuffer(asset->GetDataKey()))
						);

						if (!shaderData)
							throw std::runtime_error("Failed to parse shader data!");

						if (!shaderData->IsValid)
							throw std::runtime_error("Shader Parsing Error - " + shaderData->Error);

						std::unique_lock lock(m_WriteTaskQueueMutex);
						auto task = [this, asset, callback = eastl::move(callback), shaderData]()
							{
								Scope<ShaderProgram> result = nullptr;
								try
								{
									result = ShaderProgram::Create(asset->GetName(), shaderData);
									if (!result)
										throw std::runtime_error("Failed to create the ShaderProgram asset!");

									asset->SetAsset(eastl::move(result));
									asset->SetState(AssetState::Loaded);
									DispatchAssetEvent<AssetLoadedEvent>(asset);
								}
								catch (std::exception& e)
								{
									AR_CORE_ERROR("Shader Program Creation Error: {}", e.what());
									asset->SetState(AssetState::Failed);
									DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
								}
								if (callback)
									QueueCallback([callback = eastl::move(callback), asset]() { callback(asset); });
							};

						if (!m_MainThreadQueue)
						{
							m_WriteTaskQueue.emplace(eastl::move(task));
						}
						else
						{
							m_MainThreadQueue->SubmitTask(eastl::move(task));
						}
					}
					break;
				}
				case Utility::AssetType::MeshData:
				{
					/**
						* TODO: We need to be able to parse different formats.
						* For the time being, we are taking any asset that is
						* MeshData and parsing it in the OBJ format.
						*/
					Ref<ParsedMeshData> meshData = CreateRef<ParsedMeshData>(
						OBJParser::ParseMesh(m_MemoryDataProvider->GetDataBuffer(asset->GetDataKey()))
					);

					if (!meshData)
						throw std::runtime_error("Failed to parse Mesh Data!");

					if (!meshData->IsValid)
						throw std::runtime_error("Mesh Data Parsing Error - " + meshData->Error);

					std::unique_lock lock(m_WriteTaskQueueMutex);
					auto task = [this, asset, callback = eastl::move(callback), meshData]()
						{
							Scope<MeshData> result = nullptr;
							try
							{
								result = MeshData::Create(asset->GetName(), meshData);
								if (!result)
									throw std::runtime_error("Failed to create the MeshData asset!");

								asset->SetAsset(eastl::move(result));
								asset->SetState(AssetState::Loaded);
								DispatchAssetEvent<AssetLoadedEvent>(asset);
							}
							catch (std::exception& e)
							{
								AR_CORE_ERROR("Mesh Data Creation Error: {}", e.what());
								asset->SetState(AssetState::Failed);
								DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
							}
							if (callback)
								QueueCallback([callback = eastl::move(callback), asset]() { callback(asset); });
						};

					if (!m_MainThreadQueue)
					{
						m_WriteTaskQueue.emplace(eastl::move(task));
					}
					else
					{
						m_MainThreadQueue->SubmitTask(eastl::move(task));
					}

					break;
				}
				case Utility::AssetType::Texture:
				{
					const DataBuffer& dataBuffer = m_MemoryDataProvider->GetDataBuffer(asset->GetDataKey());

					std::unique_lock lock(m_WriteTaskQueueMutex);
					auto task = [this, asset, callback = eastl::move(callback), data = RawData(dataBuffer.GetBuffer(), dataBuffer.GetSize())]()
						{
							Scope<Texture> result = nullptr;
							try
							{
								result = Texture::Create(asset->GetName(), data);
								if (!result)
									throw std::runtime_error("Failed to create the Texture asset!");

								asset->SetAsset(eastl::move(result));
								asset->SetState(AssetState::Loaded);
								DispatchAssetEvent<AssetLoadedEvent>(asset);
							}
							catch (std::exception& e)
							{
								AR_CORE_ERROR("Texture Creation Error: {}", e.what());
								asset->SetState(AssetState::Failed);
								DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
							}
							if (callback)
								QueueCallback([callback = eastl::move(callback), asset]() { callback(asset); });
						};

					if (!m_MainThreadQueue)
					{
						m_WriteTaskQueue.emplace(eastl::move(task));
					}
					else
					{
						m_MainThreadQueue->SubmitTask(eastl::move(task));
					}

					break;
				}
				default: throw std::runtime_error("Unknown Asset Type!");
				}
			}
			catch (std::exception& e)
			{
				AR_CORE_ERROR("Asset Loading Error: {}", e.what());
				asset->SetState(AssetState::Failed);
				DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
			}
		};

		// Run task if thread pool is absent
		if (!m_ThreadPool)
			loadTask();

		// Send to thread pool if present
		else
			m_ThreadPool->SubmitTask(loadTask);
	}

	void AssetManager::QueueCallback(StoredCallbackFn&& callback)
	{
		std::unique_lock lock(m_WriteCallbackMutex);
		m_WriteCallbackQueue.emplace(eastl::move(callback));
	}

	const size_t AssetManager::GetHash(const std::type_index& type, StringView filepath, const Vector<AssetId>& dependencies, const MemoryDataKey& dataKey)
	{
		size_t seed = type.hash_code();

		if (filepath != nullptr)
			CombineHash<const char*>(seed, filepath.data());
		else if (dataKey)
			CombineHash<uint32_t>(seed, dataKey);

		for (const uint32_t& assetId : dependencies)
		{
			CombineHash<uint32_t>(seed, assetId);
		}

		return seed;
	}

	Ref<Asset> AssetManager::FindExistingAsset(const size_t& contentHash)
	{
		AssetId assetId = 0;
		std::shared_lock mapLock(m_MapMutex, std::defer_lock);
		std::shared_lock cacheLock(m_CacheMutex, std::defer_lock);

		auto mapIt = m_HashIdMap.find(contentHash);
		if (mapIt == m_HashIdMap.end())
			return nullptr;

		auto assetIt = m_AssetCache.find(mapIt->second);
		const size_t assetHash = eastl::hash<Ref<Asset>>()(assetIt->second);
		if (assetIt != m_AssetCache.end() && assetHash == contentHash)
			return assetIt->second;

		return nullptr;
	}

	template <typename AssetEventType>
	void AssetManager::DispatchAssetEvent(const Ref<Asset>& asset, StringView message)
	{
		AR_CORE_ASSERT((eastl::is_base_of_v<AssetBaseEvent, AssetEventType> == true), "AssetManager: AssetEventType must derive from AssetEvent!");

		Ref<AssetEventType> event = CreateRef<AssetEventType>(asset, message);

	#if AR_BUILD_DEBUG
		{
			if (!(event->GetCategoryFlags() & EventCategoryAsset))
			{
				AR_CORE_ASSERT(false, "AssetManager: Tried to dispatch non-asset event!");
				throw std::invalid_argument("Event doesn't belong to EventCategoryAsset!");
			}
		}
	#endif

		{
			std::shared_lock lock(m_ListenerMutex);
			if (m_Listeners.empty() && !m_EventCallback)
				return;
		}

		if (m_EventCallback)
			m_EventCallback(*event);

		Vector<Pair<EventCallbackFn, Ref<AssetEventType>>> callbacks;
		{
			std::shared_lock lock(m_ListenerMutex);
			callbacks.reserve(m_GlobalListeners.size() + 1);

			for (auto& listener : m_GlobalListeners)
			{
				if (auto it = m_Listeners.find(listener); it != m_Listeners.end())
				{
					callbacks.emplace_back(it->second, event);
				}
			}

			const StringView assetName = asset->GetName();
			for (auto listener : m_ListenerOrder)
			{
				if (m_GlobalListeners.count(listener))
					continue;

				if (auto it = m_ListenerNameMap.find(listener); it != m_ListenerNameMap.end() && it->second == assetName)
				{
					if (auto funcIt = m_Listeners.find(listener); funcIt != m_Listeners.end())
					{
						callbacks.emplace_back(funcIt->second, event);
					}
				}
			}
		}

		for (auto& [func, evt] : callbacks)
		{
			QueueCallback([func, evt]() { func(*evt); });
		}
	}

}