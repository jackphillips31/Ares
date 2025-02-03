#include <arespch.h>
#include "Engine/Data/AssetManager.h"

#include <EASTL/functional.h>

#include "Engine/Core/Application.h"
#include "Engine/Core/ThreadPool.h"
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

namespace Ares::Systems {

	Scope<AssetManager> AssetManager::Create(Systems::ThreadPool* threadPool)
	{
		return Scope<AssetManager>(new AssetManager(threadPool));
	}

	/*
	Scope<AssetManager, Internal::Deleter> AssetManager::Create(const Internal::AppAllocator& alloc, Systems::ThreadPool* threadPool)
	{
		return Scope<AssetManager, Internal::Deleter>(new AssetManager(threadPool), Internal::Deleter(alloc));
	}
	*/

	AssetManager::AssetManager(Systems::ThreadPool* threadPool)
		: m_NextAssetId(1), m_NextListenerId(1), m_ThreadPool(threadPool), m_MemoryDataProvider()
	{
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
			std::unique_lock lock1(m_ReadCallbackMutex);
			std::unique_lock lock2(m_WriteCallbackMutex);
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

	void AssetManager::Unstage(const Ref<Asset>& asset)
	{
#if AR_BUILD_DEBUG || AR_BUILD_RELEASE
		// Check for references outside AssetManager
		if (asset.use_count() > 1)
		{
			AR_CORE_WARN("Asset: {} has {} reference(s) outside the Asset Cache!", asset->GetName(), asset.use_count() - 1);
		}

		// Check to see if asset has been unloaded first
		if (asset->GetState() == AssetState::Loaded)
		{
			AR_CORE_WARN("Asset: {} has not been unloaded! Current State: {} - Asset will be removed regardless...", asset->GetName(), asset->GetStateString());
			Unload(asset);
		}
#endif

		// Remove from secondary lookup maps
		{
			std::unique_lock lock(m_MapMutex);
			m_NameIdMap.erase(asset->GetName().c_str());
			m_HashIdMap.erase(std::hash<Asset>()(asset));
		}

		// Remove from asset cache
		{
			std::unique_lock lock(m_CacheMutex);
			m_AssetCache.erase(asset->GetAssetId());
		}

		// Remove data from memory provider
		if (asset->GetDataKey())
		{
			m_MemoryDataProvider.UnregisterData(asset->GetDataKey());
		}

		asset->Unstage();

		DispatchAssetEvent<AssetUnstagedEvent>(asset);
	}

	void AssetManager::Load(const Ref<Asset>& asset, AssetCallbackFn&& callback)
	{
		if (!asset)
		{
			AR_CORE_ASSERT(false, "Asset is either corrupted or not staged!");
			throw std::invalid_argument("Asset is either corrupted or not staged!");
		}
		if (asset->GetState() == AssetState::Staged)
		{
			// Load standalone asset with no dependencies
			if (asset->GetDependencies().size() == 0)
			{
				LoadRawAsset(asset, eastl::move(callback));
			}
			// Load asset with dependencies
			else
			{
				eastl::vector<AssetId> assetDependencies = asset->GetDependencies();
				for (AssetId& currentId : assetDependencies)
				{
					Ref<Asset> currentDep = GetAsset(currentId);

					// Load staged dependency recursively
					if (currentDep->GetState() == AssetState::Staged)
					{
						LoadRawAsset(currentDep, [this, asset, callback](Ref<Asset> asset) mutable { Load(asset, eastl::move(callback)); });
						return;
					}
					// Log if dependency failed
					else if (currentDep->GetState() == AssetState::Failed)
					{
						AR_CORE_ERROR("Asset Dependency: {} failed to load!", currentDep->GetName());
					}
				}
				// Load the asset after dependencies
				LoadRawAsset(asset, eastl::move(callback));
			}
		}
	}

	void AssetManager::Load(const eastl::vector<Ref<Asset>>& assets, AssetCallbackFn&& callback)
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
			m_MemoryDataProvider.UnregisterData(asset->GetDataKey());
			asset->SetDataKey(0);
		}
		asset->Unload();
		DispatchAssetEvent<AssetUnloadedEvent>(asset);
	}

	void AssetManager::Unload(const eastl::vector<Ref<Asset>>& assets)
	{
		for (const Ref<Asset>& asset : assets)
			Unload(asset);
	}

	Ref<Asset> AssetManager::GetAsset(const eastl::string& name)
	{
		AssetId cacheId = 0;
		{
			std::shared_lock lock(m_MapMutex);
			auto it = m_NameIdMap.find(name);
			if (it != m_NameIdMap.end())
				cacheId = it->second;
			else
				return nullptr;
		}
		return GetAsset(cacheId);
	}

	Ref<Asset> AssetManager::GetAsset(const AssetId& assetId)
	{
		std::shared_lock lock(m_CacheMutex);
		auto it = m_AssetCache.find(assetId);
		if (it != m_AssetCache.end())
			return it->second;
		else
			return nullptr;
	}

	eastl::vector<Ref<Asset>> AssetManager::GetCompleteList()
	{
		eastl::vector<Ref<Asset>> result;
		std::shared_lock lock(m_CacheMutex);
		result.reserve(m_AssetCache.size());
		for (auto& entry : m_AssetCache)
		{
			result.push_back(entry.second);
		}
		return result;
	}

	const AssetListener AssetManager::AddListener(const eastl::string& name, AssetListenerCallbackFn&& callback)
	{
		std::unique_lock lock(m_ListenerMutex);
		AssetListener currentId = m_NextListenerId++;
		m_Listeners[currentId] = eastl::move(callback);
		m_ListenerNameMap[currentId] = name;
		m_ListenerOrder.push_back(currentId);
		return currentId;
	}

	const AssetListener AssetManager::AddListener(AssetListenerCallbackFn&& callback)
	{
		std::unique_lock lock(m_ListenerMutex);
		AssetListener currentId = m_NextListenerId++;
		m_Listeners[currentId] = eastl::move(callback);
		m_GlobalListeners.insert(currentId);
		m_ListenerOrder.push_back(currentId);
		return currentId;
	}

	void AssetManager::RemoveListener(AssetListener& listenerId)
	{
		// Check to see if listener is valid
		if (!listenerId)
		{
			AR_CORE_WARN("Asset Listener has already been removed!");
			return;
		}

		bool isRemoved = false;

		// Remove listener
		{
			std::unique_lock lock(m_ListenerMutex);

			// Check to see if listener exists
			auto listenerIt = m_Listeners.find(listenerId);
			if (listenerIt != m_Listeners.end())
			{
				// Erase from global listeners
				m_GlobalListeners.erase(listenerId);

				// Erase from listener secondary maps
				m_ListenerNameMap.erase(listenerId);

				// Erase listener
				m_Listeners.erase(listenerIt);

				isRemoved = true;
			}
		}

		if (!isRemoved)
		{
			AR_CORE_WARN("Did not find any asset listeners with id: {}", listenerId);
		}
		else
		{
			listenerId = 0;
		}
	}

	void AssetManager::OnUpdate(Timestep& ts)
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
			eastl::function<void()> callback = eastl::move(m_ReadCallbackQueue.front());
			m_ReadCallbackQueue.pop();
			callback();
		}
	}

	void AssetManager::QueueCallback(StoredCallbackFn&& callback)
	{
		std::unique_lock lock(m_WriteCallbackMutex);
		m_WriteCallbackQueue.emplace(eastl::move(callback));
	}

	void AssetManager::LoadRawAsset(const Ref<Asset>& asset, AssetCallbackFn&& callback)
	{
		// Mark asset's state as "Loading"
		asset->SetState(AssetState::Loading);

		// Dispatch AssetLoadingEvent
		DispatchAssetEvent<AssetLoadingEvent>(asset);

		// Create loading task
		auto loadTask = [this, asset, callback]() mutable
		{
			eastl::string eventMessage;

			// Retrieve the asset type ID to determine how to process the asset
			const Utility::AssetType assetType = Utility::GetAssetType(asset->GetType());
			try
			{
				// Check if asset is valid
				if (asset->GetDependencies().size() == 0 && !asset->HasFilepath() && !asset->GetDataKey())
				{
					eastl::string errorMsg("Staged [" + asset->GetName() + "] " + asset->GetTypeName() + " asset doesn't have valid metadata (no dependencies, no filepath, and no MemoryDataKey)!");
					throw std::runtime_error(errorMsg.c_str());
				}

				// Load file (if provided) into MemoryDataProvider
				if (asset->HasFilepath())
				{
					DataBuffer fileData = FileIO::LoadFile(asset->GetFilepath().c_str());
					MemoryDataKey fileKey = m_MemoryDataProvider.RegisterData(eastl::move(fileData));
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
					const DataBuffer& dataBuffer = m_MemoryDataProvider.GetDataBuffer(dataKey);
					std::string_view shaderData(static_cast<const char*>(dataBuffer.GetBuffer()), dataBuffer.GetSize());

					std::unique_lock lock(m_WriteTaskQueueMutex);
					m_WriteTaskQueue.emplace([this, asset, callback = eastl::move(callback), shaderData, assetType]()
						{
							Scope<Shader> result = nullptr;
							try
							{
								switch (assetType)
								{
								case Utility::AssetType::VertexShader:
								{
									result = VertexShader::Create(asset->GetName().c_str(), shaderData);
									break;
								}
								case Utility::AssetType::FragmentShader:
								{
									result = FragmentShader::Create(asset->GetName().c_str(), shaderData);
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
						});
					break;
				}
				case Utility::AssetType::ShaderProgram:
				{
					if (asset->GetDependencies().size() > 0)
					{
						std::vector<Shader*> shaders;
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
						m_WriteTaskQueue.emplace([this, asset, callback = eastl::move(callback), shaders]()
							{
								Scope<ShaderProgram> result = nullptr;
								try
								{
									result = ShaderProgram::Create(asset->GetName().c_str(), shaders);

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
							});
					}
					else
					{
						Ref<ParsedShaderData> shaderData = CreateRef<ParsedShaderData>(
							ShaderParser::ParseShaders(m_MemoryDataProvider.GetDataBuffer(asset->GetDataKey()))
						);

						if (!shaderData)
							throw std::runtime_error("Failed to parse shader data!");

						if (!shaderData->IsValid)
							throw std::runtime_error("Shader Parsing Error - " + shaderData->Error);

						std::unique_lock lock(m_WriteTaskQueueMutex);
						m_WriteTaskQueue.emplace([this, asset, callback = eastl::move(callback), shaderData]()
							{
								Scope<ShaderProgram> result = nullptr;
								try
								{
									result = ShaderProgram::Create(asset->GetName().c_str(), shaderData);
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
							});
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
						OBJParser::ParseMesh(m_MemoryDataProvider.GetDataBuffer(asset->GetDataKey()))
					);

					if (!meshData)
						throw std::runtime_error("Failed to parse Mesh Data!");

					if (!meshData->IsValid)
						throw std::runtime_error("Mesh Data Parsing Error - " + meshData->Error);

					std::unique_lock lock(m_WriteTaskQueueMutex);
					m_WriteTaskQueue.emplace([this, asset, callback = eastl::move(callback), meshData]()
						{
							Scope<MeshData> result = nullptr;
							try
							{
								result = MeshData::Create(asset->GetName().c_str(), meshData);
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
						});
					break;
				}
				case Utility::AssetType::Texture:
				{
					const DataBuffer& dataBuffer = m_MemoryDataProvider.GetDataBuffer(asset->GetDataKey());

					std::unique_lock lock(m_WriteTaskQueueMutex);
					m_WriteTaskQueue.emplace([this, asset, callback = eastl::move(callback), data = RawData(dataBuffer.GetBuffer(), dataBuffer.GetSize())]()
						{
							Scope<Texture> result = nullptr;
							try
							{
								result = Texture::Create(asset->GetName().c_str(), data);
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
						});
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

	const size_t AssetManager::GetHash(const std::type_index& type, const eastl::string& filepath, const eastl::vector<AssetId>& dependencies, const MemoryDataKey& dataKey)
	{
		size_t seed = type.hash_code();

		if (!filepath.empty())
			CombineHashEASTL<eastl::string>(seed, filepath);
		else if (dataKey)
			CombineHashEASTL<uint32_t>(seed, dataKey);

		for (const uint32_t& assetId : dependencies)
		{
			CombineHashEASTL<uint32_t>(seed, assetId);
		}

		return seed;
	}

	Ref<Asset> AssetManager::FindExistingAsset(const size_t& contentHash)
	{
		AssetId assetId = 0;
		{
			std::shared_lock lock(m_MapMutex);
			auto it = m_HashIdMap.find(contentHash);
			if (it != m_HashIdMap.end())
				assetId = it->second;
			else
				return nullptr;
		}
		{
			std::shared_lock lock(m_CacheMutex);
			auto it = m_AssetCache.find(assetId);
			if (it != m_AssetCache.end() && eastl::hash<Ref<Asset>>()(it->second) == contentHash)
				return it->second;
			else
				return nullptr;
		}

		return nullptr;
	}

}