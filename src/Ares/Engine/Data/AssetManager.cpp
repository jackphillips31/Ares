#include <arespch.h>

#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Utility.h"
#include "Engine/Data/FileIO.h"

#include "Engine/Data/AssetManager.h"

namespace Ares {

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
	std::unordered_map<std::string, std::unordered_map<uint32_t, AssetManager::ListenerCallbackFn>> AssetManager::s_Listeners;
	std::unordered_map<uint32_t, std::string> AssetManager::s_ListenerNameMap;
	std::unordered_map<uint32_t, AssetManager::ListenerCallbackFn> AssetManager::s_GlobalListeners;
	std::mutex AssetManager::s_ListenerMutex;

	std::queue<std::function<void()>> AssetManager::s_ListenerCallbackQueue;
	std::mutex AssetManager::s_ListenerCallbackQueueMutex;

	// Explicit instantiations for StageAsset types
	template Ref<Asset> AssetManager::Stage<VertexShader>(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies);
	template Ref<Asset> AssetManager::Stage<FragmentShader>(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies);
	template Ref<Asset> AssetManager::Stage<Texture>(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies);
	template Ref<Asset> AssetManager::Stage<ShaderProgram>(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies);

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
	Ref<Asset> AssetManager::Stage(const std::string& name, const std::string& filepath, const std::vector<Ref<Asset>>& dependencies)
	{
		if (filepath.empty() && dependencies.size() == 0)
		{
			AR_CORE_ASSERT(false, "Asset needs either filepath or dependencies to be staged!");
		}

		// Convert dependencies into IDs
		std::vector<uint32_t> dependencyIds;
		for (const Ref<Asset>& dep : dependencies)
		{
			dependencyIds.push_back(dep->GetAssetId());
		}

		// Compute content hash and check cache
		Ref<Asset> asset = nullptr;
		const size_t contentHash = GetHash(typeid(AssetType), filepath, dependencyIds);
		asset = FindExistingAsset(contentHash);
		if (asset)
			return asset;

		// Ensure unique asset name
		std::string assetName = name;
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			while (s_NameIdMap[assetName] != 0)
				assetName = Utility::String::IncrementStringSuffix(assetName);
		}

		// Create and initialize the asset
		const uint32_t currentId = s_NextAssetId++;
		asset = Asset::Create(typeid(AssetType), AssetState::Staged, filepath, dependencyIds);
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

		// Remove from secondary lookup maps
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			s_NameIdMap.erase(asset->GetName());
		}
		{
			std::lock_guard<std::mutex> lock(s_HashIdMutex);
			s_HashIdMap.erase(asset->GetHash());
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

		asset->SetAssetId(0);
		asset->SetAsset(nullptr);
		asset->SetState(AssetState::None);

		DispatchAssetEvent<AssetUnstagedEvent>(asset);
	}

	void AssetManager::Load(const Ref<Asset>& asset, const AssetCallbackFn callback)
	{
		Load(std::vector<Ref<Asset>>({ asset }), callback);
	}

	void AssetManager::Load(const std::vector<Ref<Asset>>& assets, const AssetCallbackFn callback)
	{
		for (const Ref<Asset>& asset : assets)
		{
			if (asset->GetState() == AssetState::Staged)
			{
				// Load standalone asset with no dependencies
				if (asset->GetDependencies().size() == 0 && !asset->GetFilepath().empty())
				{
					LoadRawAsset(asset, callback);
				}
				// Load asset with dependencies
				else if (asset->GetDependencies().size() > 0 && asset->GetFilepath().empty())
				{
					std::vector<uint32_t> assetDependencies = asset->GetDependencies();
					for (uint32_t& currentDependency : assetDependencies)
					{
						Ref<Asset> currentDep = GetAsset(currentDependency);

						// Load staged dependency recursively
						if (currentDep->GetState() == AssetState::Staged)
						{
							LoadRawAsset(currentDep, [assets, callback](Ares::Ref<Asset> asset) { Load(assets, callback); });
							return;
						}
						// Log if dependency failed
						else if (currentDep->GetState() == AssetState::Failed)
						{
							AR_CORE_ASSERT("Asset: {} failed to load!", currentDep->GetName());
						}
					}
					// Load the asset after dependencies
					LoadRawAsset(asset, callback);
				}
			}
		}
	}

	void AssetManager::Unload(const Ref<Asset>& asset)
	{
		if (asset->GetAsset().use_count() > 1)
		{
			// Asset has references outside AssetManager
			AR_CORE_WARN("Cant unload Asset! Asset: {} has {} reference(s) outside Asset Cache!", asset->GetName(), asset->GetAsset().use_count() - 1);
		}
		else
		{
			// Unload asset
			asset->SetAsset(nullptr);
			DispatchAssetEvent<AssetUnloadedEvent>(asset);
		}
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

	const AssetListener AssetManager::AddListener(const std::string& name, std::function<void(Event&)> callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		uint32_t currentId = s_NextListenerId++;
		s_Listeners[name][currentId] = std::move(callback);
		s_ListenerNameMap[currentId] = name;
		return currentId;
	}

	const AssetListener AssetManager::AddListener(std::function<void(Event&)> callback)
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

	void AssetManager::LoadRawAsset(const Ref<Asset>& asset, const AssetCallbackFn callback)
	{
		// Mark asset's state as "Loading"
		asset->SetState(AssetState::Loading);

		// Dispatch AssetLoadingEvent
		DispatchAssetEvent<AssetLoadingEvent>(asset);

		// Submit the task to the ThreadPool for async loading
		ThreadPool::SubmitTask([asset, callback]()
			{
				Ref<AssetBase> rawAsset = nullptr;
				std::string eventMessage;

				// Retrieve the asset type ID to determine how to process the asset
				const uint32_t assetTypeId = Utility::Type::GetTypeMapId(asset->GetType());
				try
				{
					// Handle asset loading based on its type
					switch (assetTypeId)
					{
					case Utility::Type::VertexShader: {
						rawAsset = LoadVertexShader(asset);
						break;
					}
					case Utility::Type::FragmentShader: {
						rawAsset = LoadFragmentShader(asset);
						break;
					}
					case Utility::Type::ShaderProgram: {
						rawAsset = LoadShaderProgram(asset);
						break;
					}
					case Utility::Type::Texture: {
						rawAsset = LoadTexture(asset);
						break;
					}
					default: {
						// Log and throw an error for unsupported asset types
						AR_CORE_ERROR("Unsupported asset type!");
						throw std::runtime_error("Unsupported asset type!");
					}
					}

					// If loading succeeds, assign the loaded asset and mark asset state as "Loaded"
					asset->SetAsset(rawAsset);
					asset->SetState(AssetState::Loaded);
				}
				catch (std::exception& e)
				{
					// If an exception occurs, mark the asset state as "Failed" and store error message
					asset->SetState(AssetState::Failed);
					eventMessage = e.what();
				}

				// Dispatch event and notify listeners of asset's loading status
				if (asset->GetState() == AssetState::Loaded)
					DispatchAssetEvent<AssetLoadedEvent>(asset, eventMessage);
				else if (asset->GetState() == AssetState::Failed)
					DispatchAssetEvent<AssetFailedEvent>(asset, eventMessage);
				else
				{
					AR_CORE_ASSERT(false, "Something went wrong when loading asset!");
				}

				// If a callback function is provided, enqueue it for execution
				if (callback)
					QueueCallback([callback, asset]() { callback(asset); });
			});
	}

	Ref<VertexShader> AssetManager::LoadVertexShader(const Ref<Asset>& asset)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(asset->GetFilepath());
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Vertex Shader file: '" + asset->GetFilepath() + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return VertexShader::Create(asset->GetName(), fileBuffer);
	}

	Ref<FragmentShader> AssetManager::LoadFragmentShader(const Ref<Asset>& asset)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(asset->GetFilepath());
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Fragment Shader file: '" + asset->GetFilepath() + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return FragmentShader::Create(asset->GetName(), fileBuffer);
	}

	Ref<Texture> AssetManager::LoadTexture(const Ref<Asset>& asset)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(asset->GetFilepath());
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Texture file: '" + asset->GetFilepath() + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return Texture::Create(asset->GetName(), fileBuffer);
	}

	Ref<ShaderProgram> AssetManager::LoadShaderProgram(const Ref<Asset>& asset)
	{
		if (asset->GetDependencies().size() > 0 && asset->GetFilepath().empty())
		{
			// Load ShaderProgram using its dependencies
			std::vector<uint32_t> depIds = asset->GetDependencies();
			std::vector<Ref<Asset>> dependencies;
			std::vector<Ref<Shader>> tempShaders;

			for (uint32_t& dep : depIds)
			{
				dependencies.push_back(GetAsset(dep));
			}

			// Extract shaders from dependencies
			for (Ref<Asset>& dep : dependencies)
			{
				if (dep->GetType() == typeid(VertexShader) || dep->GetType() == typeid(FragmentShader))
					tempShaders.push_back(dep->GetAsset<Shader>());
			}

			if (tempShaders.size() == 0)
			{
				std::string error("None of this Shader Program's dependencies were shaders!");
				AR_CORE_ERROR(error);
				throw std::runtime_error(error);
			}

			// Create ShaderProgram from extracted shaders
			Ref<ShaderProgram> rawAsset = ShaderProgram::Create(asset->GetName(), tempShaders);

			if (rawAsset == nullptr)
			{
				std::string error("Some error occurred when creating Shader Program!");
				AR_CORE_ERROR(error);
				throw std::runtime_error(error);
			}
			else
				return rawAsset;
		}
		else if (!asset->GetFilepath().empty() && asset->GetDependencies().size() == 0)
		{
			// Load ShaderProgram from a single file
			FileBuffer fileBuffer = FileIO::LoadFile(asset->GetFilepath());
			if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
			{
				std::string error = "Failed to load Shader Program file: '" + asset->GetFilepath() + "'";
				AR_CORE_ERROR(error);
				throw std::runtime_error(error);
			}

			std::string shaderSource(static_cast<const char*>(fileBuffer.GetBuffer()), fileBuffer.GetSize());
			std::string vertexSource = "";
			std::string fragmentSource = "";

			// Parse shader types from source code
			const char* typeToken = "#type";
			size_t typeTokenLength = strlen(typeToken);
			size_t pos = shaderSource.find(typeToken, 0);
			while (pos != std::string::npos)
			{
				size_t eol = shaderSource.find_first_of("\r\n", pos);
				if (eol == std::string::npos)
					throw std::runtime_error("Shader Syntax Error!");

				size_t begin = pos + typeTokenLength + 1;
				std::string type = shaderSource.substr(begin, eol - begin);
				if (type != "vertex" && type != "fragment" && type != "pixel")
					throw std::runtime_error("Shader Syntax Error: Invalid shader type specification!");

				size_t nextLinePos = shaderSource.find_first_not_of("\r\n", eol);
				if (nextLinePos == std::string::npos)
					throw std::runtime_error("Shader Syntax Error!");

				pos = shaderSource.find(typeToken, nextLinePos);

				if (type == "vertex")
				{
					vertexSource = (pos == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, pos - nextLinePos);
				}
				else if (type == "fragment" || type == "pixel")
				{
					fragmentSource = (pos == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, pos - nextLinePos);
				}
			}

			// Ensure both vertex and fragment shaders exist
			if (vertexSource == "" || fragmentSource == "")
				throw std::runtime_error("One or more shaders are missing!");

			// Create ShaderProgram using parsed shaders
			Ref<ShaderProgram> rawAsset = ShaderProgram::Create(asset->GetName(), {
				VertexShader::Create(asset->GetName() + "_Vertex", vertexSource),
				FragmentShader::Create(asset->GetName() + "_Fragment", fragmentSource)
			});

			if (rawAsset == nullptr)
				throw std::runtime_error("Some error occurred when created Shader Program!");
			else
				return rawAsset;
		}
		else
		{
			// Handle unsupported cases with both filepath and dependencies
			std::string error("LoadShaderProgram doesn't support a Shader Program with a filepath AND dependencies!");
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
	}

	const size_t AssetManager::GetHash(const std::type_index& type, const std::string& filepath, const std::vector<uint32_t>& dependencies)
	{
		size_t hash = 0;

		// Hash filepath (if not empty)
		if (!filepath.empty())
		{
			CombineHash<std::string>(hash, filepath);
		}

		// Hash dependency ids (if not empty)
		for (uint32_t id : dependencies)
		{
			CombineHash<uint32_t>(hash, id);
		}

		// Hash type
		CombineHash<std::type_index>(hash, type);

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
				if (assetIt->second->GetHash() == contentHash)
				{
					return assetIt->second;
				}
			}
		}

		return nullptr;
	}

}