#include <arespch.h>
#include "Engine/Data/AssetManager.h"

#include "Engine/Core/MainThreadQueue.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Utility.h"
#include "Engine/Data/DataBuffer.h"
#include "Engine/Data/FileIO.h"
#include "Engine/Data/MemoryDataProvider.h"
#include "Engine/Data/RawData.h"
#include "Engine/Data/Parsers/OBJParser.h"
#include "Engine/Data/Parsers/ShaderParser.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Renderer/Assets/MeshData.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Assets/Texture.h"

namespace Ares {

	void AssetManager::Init()
	{
		AR_CORE_INFO("Initializing AssetManager");
		Shutdown();
	}

	void AssetManager::Shutdown()
	{
		// Clear the asset cache
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			s_AssetCache.clear();
			s_NextAssetId = 1;
		}

		// Clear secondary maps
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			s_NameIdMap.clear();
		}
		{
			std::lock_guard<std::mutex> lock(s_HashIdMutex);
			s_HashIdMap.clear();
		}
		{
			std::lock_guard<std::mutex> lock(s_TypeIdMutex);
			s_TypeIdMap.clear();
		}

		// Clear callback queue
		{
			std::lock_guard<std::mutex> lock(s_CallbackQueueMutex);
			while (!s_CallbackQueue.empty()) s_CallbackQueue.pop();
		}

		// Clear listener callback queue
		{
			std::lock_guard<std::mutex> lock(s_ListenerCallbackQueueMutex);
			while (!s_ListenerCallbackQueue.empty()) s_ListenerCallbackQueue.pop();
		}

		// Clear all listeners
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);
			s_Listeners.clear();
			s_GlobalListeners.clear();
		}
	}

	template <typename AssetType>
	Ref<Asset> AssetManager::Stage(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies, const MemoryDataKey dataKey)
	{
		if (filepath.empty() && dependencies.size() == 0 && dataKey == 0)
		{
			AR_CORE_ASSERT(false, "Asset needs either filepath, dependencies or raw data to be staged!");
		}

		// Convert dependencies into IDs
		std::vector<uint32_t> dependencyIds;
		for (const Ref<Asset>& dep : dependencies)
		{
			dependencyIds.push_back(dep->GetAssetId());
		}

		// Compute content hash and check cache
		Ref<Asset> asset = nullptr;
		const size_t contentHash = GetHash(typeid(AssetType), filepath, dependencyIds, dataKey);
		asset = FindExistingAsset(contentHash);
		if (asset)
			return asset;

		// Ensure unique asset name
		std::string assetName = name;
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			while (s_NameIdMap[assetName] != 0)
				assetName = Utility::IncrementStringSuffix(assetName);
		}

		// Create and initialize the asset
		const uint32_t currentId = s_NextAssetId++;
		asset = Asset::Create(typeid(AssetType), AssetState::Staged, filepath, dependencyIds, dataKey);
		asset->SetName(assetName);
		asset->SetAssetId(currentId);

		// Update maps for name, hash, and type associations
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			s_NameIdMap[assetName] = currentId;
		}
		{
			std::lock_guard<std::mutex> lock(s_HashIdMutex);
			s_HashIdMap[contentHash] = currentId;
		}
		{
			std::lock_guard<std::mutex> lock(s_TypeIdMutex);
			s_TypeIdMap[typeid(AssetType)] = currentId;
		}

		// Add asset to cache
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			s_AssetCache[currentId] = asset;
		}

		// Dispatch Event, notify listeners, and return
		DispatchAssetEvent<AssetStagedEvent>(asset);
		return asset;
	}

	void AssetManager::Unstage(const Ref<Asset>& asset)
	{
		// Check for references outside AssetManager
		if (asset.use_count() > 1)
		{
			AR_CORE_WARN("Asset: {} has {} reference(s) outside Asset Cache!", asset->GetName(), asset.use_count() - 1);
		}

		// Check to see if asset has been unloaded first
		if (asset->GetAsset() != nullptr)
		{
			AR_CORE_WARN("Asset: {} has not been unloaded! Current State: {} - Asset will be removed regardless...", asset->GetName(), asset->GetStateString());
		}

		// Remove from secondary lookup maps
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			s_NameIdMap.erase(asset->GetName());
		}
		{
			std::lock_guard<std::mutex> lock(s_HashIdMutex);
			s_HashIdMap.erase(std::hash<Asset>()(asset));
		}
		{
			std::lock_guard<std::mutex> lock(s_TypeIdMutex);
			s_TypeIdMap.erase(asset->GetType());
		}

		// Remove from asset cache
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			s_AssetCache.erase(asset->GetAssetId());
		}

		asset->Unstage();

		DispatchAssetEvent<AssetUnstagedEvent>(asset);
	}

	void AssetManager::Load(const Ref<Asset>& asset, AssetCallbackFn&& callback)
	{
		Load(std::vector<Ref<Asset>>({ asset }), std::move(callback));
	}

	void AssetManager::Load(const std::vector<Ref<Asset>>& assets, AssetCallbackFn&& callback)
	{
		for (const Ref<Asset>& asset : assets)
		{
			if (asset == nullptr)
			{
				AR_CORE_ASSERT(false, "Asset has not been staged!");
				continue;
			}
			if (asset->GetState() == AssetState::Staged)
			{
				// Load standalone asset with no dependencies
				if (asset->GetDependencies().size() == 0)
				{
					LoadRawAsset(asset, std::move(callback));
				}
				// Load asset with dependencies
				else
				{
					std::vector<uint32_t> assetDependencies = asset->GetDependencies();
					for (uint32_t& currentDependency : assetDependencies)
					{
						Ref<Asset> currentDep = GetAsset(currentDependency);

						// Load staged dependency recursively
						if (currentDep->GetState() == AssetState::Staged)
						{
							LoadRawAsset(currentDep, [assets, callback](Ares::Ref<Asset> asset) mutable { Load(assets, std::move(callback)); });
							return;
						}
						// Log if dependency failed
						else if (currentDep->GetState() == AssetState::Failed)
						{
							AR_CORE_ASSERT(false, "Asset: {} failed to load!", currentDep->GetName());
						}
					}
					// Load the asset after dependencies
					LoadRawAsset(asset, std::move(callback));
				}
			}
		}
	}

	void AssetManager::Unload(const Ref<Asset>& asset)
	{
		// Unload asset
		if (asset->HasFilepath() && asset->GetDataKey() != 0)
		{
			MemoryDataProvider::UnregisterData(asset->GetDataKey());
		}
		asset->Unload();
		DispatchAssetEvent<AssetUnloadedEvent>(asset);
	}

	Ref<Asset> AssetManager::GetAsset(const std::string& name)
	{
		uint32_t cachedId = 0;
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			auto it = s_NameIdMap.find(name);
			if (it != s_NameIdMap.end())
				cachedId = it->second;
			else
				return nullptr;
		}
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			auto it = s_AssetCache.find(cachedId);
			if (it != s_AssetCache.end())
				return it->second;
			else
				return nullptr;
		}
	}

	Ref<Asset> AssetManager::GetAsset(const uint32_t& assetId)
	{
		std::lock_guard<std::mutex> lock(s_CacheMutex);
		auto it = s_AssetCache.find(assetId);
		if (it != s_AssetCache.end())
			return it->second;
		else
			return nullptr;
	}
	
	std::vector<Ref<Asset>> AssetManager::GetCompleteList()
	{
		std::vector<Ref<Asset>> result;
		std::unordered_map<uint32_t, Ref<Asset>> tempCache;
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			tempCache = s_AssetCache;
		}
		for (auto& entry : tempCache)
		{
			result.emplace_back(entry.second);
		}
		return result;
	}

	const AssetListener AssetManager::AddListener(const std::string& name, AssetListenerCallbackFn&& callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		uint32_t currentId = s_NextListenerId++;
		s_Listeners[name][currentId] = std::move(callback);
		s_ListenerNameMap[currentId] = name;
		return currentId;
	}

	const AssetListener AssetManager::AddListener(AssetListenerCallbackFn&& callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		uint32_t currentId = s_NextListenerId++;
		s_GlobalListeners[currentId] = std::move(callback);
		return currentId;
	}

	void AssetManager::RemoveListener(AssetListener& listenerId)
	{
		// Check to see if Listener is valid
		if (!listenerId)
		{
			AR_CORE_WARN("Asset listener has already been removed!");
			return;
		}

		// Remove listener
		{
			size_t listenersRemoved = 0;
			std::lock_guard<std::mutex> lock(s_ListenerMutex);

			// Find asset name for this listener if not global
			auto nameIt = s_ListenerNameMap.find(listenerId);
			if (nameIt != s_ListenerNameMap.end())
			{
				// Erase name based listener
				auto& listeners = s_Listeners[nameIt->second];
				listenersRemoved += listeners.erase(listenerId);
				s_ListenerNameMap.erase(nameIt);
			}
			else
			{
				// Erase global listener
				listenersRemoved += s_GlobalListeners.erase(listenerId);
			}

			// Check to see if a listener was removed
			if (!listenersRemoved)
			{
				AR_CORE_WARN("Did not find any asset listeners with id: {}", listenerId);
			}
			else
				listenerId = 0;
		}
	}

	void AssetManager::OnUpdate()
	{
		ProcessCallbacks();
		ProcessListenerCallbacks();
	}

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

	void AssetManager::QueueListenerCallback(std::function<void()> callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerCallbackQueueMutex);
		s_ListenerCallbackQueue.emplace(std::move(callback));
	}

	void AssetManager::QueueCallback(std::function<void()> callback)
	{
		std::lock_guard<std::mutex> lock(s_CallbackQueueMutex);
		s_CallbackQueue.emplace(std::move(callback));
	}

	void AssetManager::ProcessListenerCallbacks()
	{
		std::lock_guard<std::mutex> lock(s_ListenerCallbackQueueMutex);
		while (!s_ListenerCallbackQueue.empty())
		{
			auto callback = std::move(s_ListenerCallbackQueue.front());
			s_ListenerCallbackQueue.pop();
			callback();
		}
	}

	void AssetManager::ProcessCallbacks()
	{
		std::lock_guard<std::mutex> lock(s_CallbackQueueMutex);
		while (!s_CallbackQueue.empty())
		{
			auto callback = std::move(s_CallbackQueue.front());
			s_CallbackQueue.pop();
			callback();
		}
	}

	void AssetManager::LoadRawAsset(const Ref<Asset>& asset, AssetCallbackFn&& callback)
	{
		// Mark asset's state as "Loading"
		asset->SetState(AssetState::Loading);

		// Dispatch AssetLoadingEvent
		DispatchAssetEvent<AssetLoadingEvent>(asset);

		// Submit the task to the ThreadPool for async loading
		ThreadPool::SubmitTask([asset, callback]() mutable
		{
			//Scope<AssetBase> rawAsset = nullptr;
			std::string eventMessage;

			// Retrieve the asset type ID to determine how to process the asset
			const Utility::AssetType assetType = Utility::GetAssetType(asset->GetType());
			try
			{
				// Check if asset is valid
				if (asset->GetDependencies().size() == 0 && !asset->HasFilepath() && asset->GetDataKey() == 0)
				{
					throw std::runtime_error("Staged [" + asset->GetName() + "] " + asset->GetTypeName() + " asset doesn't have valid metadata (no dependencies & no filepath & no MemoryDataKey)");
				}

				// Load file (if provided) into MemoryDataProvider
				if (asset->HasFilepath())
				{
					DataBuffer fileData = FileIO::LoadFile(asset->GetFilepath());
					MemoryDataKey fileKey = MemoryDataProvider::RegisterData(std::move(fileData));
					if (fileKey == 0)
					{
						throw std::runtime_error("Something went wrong... Asset DataKey is still 0!");
					}
					asset->SetDataKey(fileKey);
				}

				// Handle asset loading based on its type
				if (assetType == Utility::AssetType::VertexShader || assetType == Utility::AssetType::FragmentShader)
				{
					const DataBuffer& data = MemoryDataProvider::GetData(asset->GetDataKey());
					std::string_view shaderData(static_cast<const char*>(data.GetBuffer()), data.GetSize());

					MainThreadQueue::SubmitTask([asset, callback = std::move(callback), shaderData, assetType]() {
						Scope<Shader> result = nullptr;
						try
						{
							if (assetType == Utility::AssetType::VertexShader)
							{
								result = VertexShader::Create(asset->GetName(), shaderData);
							}
							else if (assetType == Utility::AssetType::FragmentShader)
							{
								result = FragmentShader::Create(asset->GetName(), shaderData);
							}

							if (result == nullptr)
							{
								throw std::runtime_error("Something went wrong when creating the raw asset!");
							}

							if (assetType == Utility::AssetType::VertexShader)
							{
								asset->SetAsset(Scope<VertexShader>(static_cast<VertexShader*>(result.release())));
							}
							else if (assetType == Utility::AssetType::FragmentShader)
							{
								asset->SetAsset(Scope<FragmentShader>(static_cast<FragmentShader*>(result.release())));
							}
							else
							{
								throw std::runtime_error("Asset was not set!");
							}

							asset->SetState(AssetState::Loaded);
							DispatchAssetEvent<AssetLoadedEvent>(asset);
						}
						catch (std::exception& e)
						{
							AR_CORE_CRITICAL("{} Creation Error: {}", asset->GetTypeName(), e.what());
							asset->SetState(AssetState::Failed);
							DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
						}
						if (callback)
							QueueCallback([callback, asset]() { callback(asset); });
					});
				}
				else if (assetType == Utility::AssetType::ShaderProgram)
				{
					if (asset->GetDependencies().size() > 0)
					{
						std::vector<Shader*> shaders;
						for (const uint32_t& assetId : asset->GetDependencies())
						{
							Ref<Asset> dependency = AssetManager::GetAsset(assetId);
							if (dependency->GetState() != AssetState::Loaded)
								throw std::runtime_error("Shader Program dependency is not loaded!");

							if (dependency->GetType() == typeid(VertexShader))
							{
								shaders.push_back(dependency->GetAsset<VertexShader>());
								continue;
							}
							if (dependency->GetType() == typeid(FragmentShader))
							{
								shaders.push_back(dependency->GetAsset<FragmentShader>());
								continue;
							}
						}
						MainThreadQueue::SubmitTask([asset, callback = std::move(callback), shaders]() {
							Scope<ShaderProgram> result = nullptr;
							try
							{
								result = ShaderProgram::Create(asset->GetName(), shaders);
								if (result == nullptr)
									throw std::runtime_error("Something went wrong when creating the raw asset!");

								asset->SetAsset(std::move(result));
								asset->SetState(AssetState::Loaded);
								DispatchAssetEvent<AssetLoadedEvent>(asset);
							}
							catch (std::exception& e)
							{
								AR_CORE_CRITICAL("Shader Program Creation Error: {}", e.what());
								asset->SetState(AssetState::Failed);
								DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
							}
							if (callback)
								QueueCallback([callback, asset]() { callback(asset); });
						});
					}
					else
					{
						const DataBuffer& data = MemoryDataProvider::GetData(asset->GetDataKey());
						Ref<ParsedShaderData> shaderData = CreateRef<ParsedShaderData>(ShaderParser::ParseShaders(data));

						if (shaderData == nullptr)
							throw std::runtime_error("Shader Data was not parsed!");

						if (!shaderData->IsValid)
							throw std::runtime_error("Error while parsing shaders: " + shaderData->Error);

						MainThreadQueue::SubmitTask([asset, callback = std::move(callback), shaderData]() {
							Scope<ShaderProgram> result = nullptr;
							try
							{
								result = ShaderProgram::Create(asset->GetName(), shaderData);
								if (result == nullptr)
									throw std::runtime_error("Something went wrong when creating the raw asset!");

								asset->SetAsset(std::move(result));
								asset->SetState(AssetState::Loaded);
								DispatchAssetEvent<AssetLoadedEvent>(asset);
							}
							catch (std::exception& e)
							{
								AR_CORE_CRITICAL("Shader Program Creation Error: {}", e.what());
								asset->SetState(AssetState::Failed);
								DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
							}
							if (callback)
								QueueCallback([callback, asset]() { callback(asset); });
						});
					}
				}
				else if (assetType == Utility::AssetType::MeshData)
				{
					const DataBuffer& data = MemoryDataProvider::GetData(asset->GetDataKey());
					Ref<ParsedMeshData> meshData = nullptr;

					if (Utility::GetFileExtension(asset->GetFilepath()) == "obj")
					{
						meshData = CreateRef<ParsedMeshData>(OBJParser::ParseMesh(data));
					}
					else
					{
						// TODO: We need to be able to parse different formats.
						//		 For the time being, we are taking any asset
						//		 thats MeshData and parsing it as if its an OBJ.
						meshData = CreateRef<ParsedMeshData>(OBJParser::ParseMesh(data));
					}

					if (meshData == nullptr)
						throw std::runtime_error("Mesh Data was not parsed!");

					if (!meshData->IsValid)
						throw std::runtime_error("Error while parsing Mesh Data: " + meshData->Error);

					MainThreadQueue::SubmitTask([asset, callback = std::move(callback), meshData]() {
						Scope<MeshData> result = nullptr;
						try
						{
							result = MeshData::Create(asset->GetName(), meshData);
							if (result == nullptr)
								throw std::runtime_error("Something went wrong when creating the raw asset!");

							asset->SetAsset(std::move(result));
							asset->SetState(AssetState::Loaded);
							DispatchAssetEvent<AssetLoadedEvent>(asset);
						}
						catch (std::exception& e)
						{
							AR_CORE_CRITICAL("Mesh Data Creation Error: {}", e.what());
							asset->SetState(AssetState::Failed);
							DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
						}
						if (callback)
							QueueCallback([callback, asset]() { callback(asset); });
					});
				}
				else if (assetType == Utility::AssetType::Texture)
				{
					const DataBuffer& data = MemoryDataProvider::GetData(asset->GetDataKey());

					MainThreadQueue::SubmitTask([asset, callback = std::move(callback), data = RawData(data.GetBuffer(), data.GetSize())]() {
						Scope<Texture> result = nullptr;
						try
						{
							result = Texture::Create(asset->GetName(), data);
							if (result == nullptr)
								throw std::runtime_error("Something went wrong when creating the raw asset!");

							asset->SetAsset(std::move(result));
							asset->SetState(AssetState::Loaded);
							DispatchAssetEvent<AssetLoadedEvent>(asset);
						}
						catch (std::exception& e)
						{
							AR_CORE_CRITICAL("Texture Creation Error: {}", e.what());
							asset->SetState(AssetState::Failed);
							DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
						}
						if (callback)
							QueueCallback([callback, asset]() { callback(asset); });
					});
				}
				else
				{
					throw std::runtime_error("Unknown Asset Type!");
				}
			}
			catch (std::exception& e)
			{
				// Log any errors
				AR_CORE_CRITICAL("Asset Loading Error: {}", e.what());
				asset->SetState(AssetState::Failed);
				DispatchAssetEvent<AssetFailedEvent>(asset, e.what());
			}
		});
	}

	const size_t AssetManager::GetHash(
		const std::type_index& type,
		const std::string& filepath,
		const std::vector<uint32_t>& dependencies,
		const MemoryDataKey dataKey
	)
	{
		size_t hash = 0;

		// Hash type
		CombineHash<std::type_index>(hash, type);

		// Hash filepath (if not empty)
		if (!filepath.empty())
		{
			CombineHash<std::string>(hash, filepath);
		}
		// Only hash data key if filepath is empty
		else
		{
			if (dataKey != 0)
			{
				CombineHash<uint32_t>(hash, dataKey);
			}
		}

		// Hash dependency ids (if not empty)
		for (uint32_t id : dependencies)
		{
			CombineHash<uint32_t>(hash, id);
		}

		return hash;
	}

	Ref<Asset> AssetManager::FindExistingAsset(const size_t& contentHash)
	{
		std::unordered_map<size_t, uint32_t> tempHashIdMap;
		std::unordered_map<uint32_t, Ref<Asset>> tempAssetCache;
		{
			std::lock_guard<std::mutex> lock(s_HashIdMutex);
			tempHashIdMap = s_HashIdMap;
		}

		auto it = tempHashIdMap.find(contentHash);
		if (it != tempHashIdMap.end())
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			auto assetIt = s_AssetCache.find(it->second);
			if (assetIt != s_AssetCache.end())
			{
				if (std::hash<Asset>()(assetIt->second) == contentHash)
					return assetIt->second;
			}
		}

		return nullptr;
	}

	// Private members
	std::unordered_map<uint32_t, Ref<Asset>> AssetManager::s_AssetCache;
	std::mutex AssetManager::s_CacheMutex;
	std::atomic<uint32_t> AssetManager::s_NextAssetId{ 1 };

	std::unordered_map<std::string, uint32_t> AssetManager::s_NameIdMap;
	std::mutex AssetManager::s_NameIdMutex;
	std::unordered_map<size_t, uint32_t> AssetManager::s_HashIdMap;
	std::mutex AssetManager::s_HashIdMutex;
	std::unordered_map<std::type_index, uint32_t> AssetManager::s_TypeIdMap;
	std::mutex AssetManager::s_TypeIdMutex;

	std::queue<std::function<void()>> AssetManager::s_CallbackQueue;
	std::mutex AssetManager::s_CallbackQueueMutex;

	std::atomic<uint32_t> AssetManager::s_NextListenerId{ 1 };
	std::unordered_map<std::string, std::unordered_map<uint32_t, AssetManager::AssetListenerCallbackFn>> AssetManager::s_Listeners;
	std::unordered_map<uint32_t, std::string> AssetManager::s_ListenerNameMap;
	std::unordered_map<uint32_t, AssetManager::AssetListenerCallbackFn> AssetManager::s_GlobalListeners;
	std::mutex AssetManager::s_ListenerMutex;

	std::queue<std::function<void()>> AssetManager::s_ListenerCallbackQueue;
	std::mutex AssetManager::s_ListenerCallbackQueueMutex;

	// Template explicit instantiations
	template Ref<Asset> AssetManager::Stage<VertexShader>(const std::string&, const std::string&, const std::vector<Ref<Asset>>&, const MemoryDataKey dataKey);
	template Ref<Asset> AssetManager::Stage<FragmentShader>(const std::string&, const std::string&, const std::vector<Ref<Asset>>&, const MemoryDataKey dataKey);
	template Ref<Asset> AssetManager::Stage<ShaderProgram>(const std::string&, const std::string&, const std::vector<Ref<Asset>>&, const MemoryDataKey dataKey);
	template Ref<Asset> AssetManager::Stage<Texture>(const std::string&, const std::string&, const std::vector<Ref<Asset>>&, const MemoryDataKey dataKey);
	template Ref<Asset> AssetManager::Stage<MeshData>(const std::string&, const std::string&, const std::vector<Ref<Asset>>&, const MemoryDataKey dataKey);

	template void AssetManager::DispatchAssetEvent<AssetStagedEvent>(const Ref<Asset>&, const std::string&);
	template void AssetManager::DispatchAssetEvent<AssetUnstagedEvent>(const Ref<Asset>&, const std::string&);
	template void AssetManager::DispatchAssetEvent<AssetLoadedEvent>(const Ref<Asset>&, const std::string&);
	template void AssetManager::DispatchAssetEvent<AssetUnloadedEvent>(const Ref<Asset>&, const std::string&);
	template void AssetManager::DispatchAssetEvent<AssetFailedEvent>(const Ref<Asset>&, const std::string&);
}