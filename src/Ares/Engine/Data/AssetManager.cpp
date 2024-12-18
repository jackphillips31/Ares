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
	template Ref<Asset> AssetManager::StageAsset<VertexShader>(const std::string& name, const std::string& filepath, const std::vector<uint32_t>& dependencies);
	template Ref<Asset> AssetManager::StageAsset<FragmentShader>(const std::string& name, const std::string& filepath, const std::vector<uint32_t>& dependencies);
	template Ref<Asset> AssetManager::StageAsset<Texture2D>(const std::string& name, const std::string& filepath, const std::vector<uint32_t>& dependencies);
	template Ref<Asset> AssetManager::StageAsset<ShaderProgram>(const std::string& name, const std::string& filepath, const std::vector<uint32_t>& dependencies);

	// Explicit instantiations for LoadAsset types
	template void AssetManager::LoadAsset<VertexShader>(const std::string& name, const std::string& filepath, const AssetCallbackFn callback);
	template void AssetManager::LoadAsset<FragmentShader>(const std::string& name, const std::string& filepath, const AssetCallbackFn callback);
	template void AssetManager::LoadAsset<Texture2D>(const std::string& name, const std::string& filepath, const AssetCallbackFn callback);
	template void AssetManager::LoadAsset<ShaderProgram>(const std::string& name, const std::string& filepath, const AssetCallbackFn callback);

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
	Ref<Asset> AssetManager::StageAsset(const std::string& name, const std::string& filepath, const std::vector<uint32_t>& dependencies)
	{
		if (filepath.empty() && dependencies.size() == 0)
		{
			AR_CORE_ASSERT(false, "Asset needs either filepath or dependencies to be staged!");
		}

		Ref<Asset> asset = nullptr;
		const size_t contentHash = GetHash(typeid(AssetType), filepath, dependencies);

		// Check Cache
		asset = FindExistingAsset(contentHash);
		if (asset)
			return asset;

		// Check for duplicate names
		std::string assetName = name;
		{
			std::lock_guard<std::mutex> lock(s_NameIdMutex);
			while (s_NameIdMap[assetName] != 0)
				assetName = Utility::String::IncrementStringSuffix(assetName);
		}

		// Create asset
		const uint32_t currentId = s_NextAssetId++;
		asset = Asset::Create(typeid(AssetType), AssetState::Staged, filepath, dependencies);
		asset->SetName(assetName);
		asset->SetAssetId(currentId);

		// Insert data into secondary maps
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

		// Insert empty asset into cache
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			s_AssetCache[currentId] = asset;
		}

		// Dispatch Event, notify listeners, and return
		DispatchAssetEvent(asset);
		return asset;
	}

	template <typename AssetType>
	void AssetManager::LoadAsset(const std::string& name, const std::string& filepath, const AssetCallbackFn callback)
	{
		Ref<Asset> asset = StageAsset<AssetType>(name, filepath, {});

		// Check if asset is already loaded
		if (asset->GetState() == AssetState::Loaded)
		{
			if (callback)
				QueueCallback([callback, asset]() { callback(asset); });
			return;
		}

		// Load asset
		ThreadPool::SubmitTask([asset, callback]() {
			Ref<AssetType> rawAsset = nullptr;
			std::string eventMessage;
			std::string assetName = asset->GetName();
			std::string filepath = asset->GetFilepath();
			const uint32_t currentId = asset->GetAssetId();
			try
			{
				asset->SetState(AssetState::Loading);
				DispatchAssetEvent(asset);
				if constexpr (std::is_same<AssetType, VertexShader>::value)
				{
					rawAsset = LoadVertexShader(assetName, filepath);
				}
				else if constexpr (std::is_same<AssetType, FragmentShader>::value)
				{
					rawAsset = LoadFragmentShader(assetName, filepath);
				}
				else if constexpr (std::is_same<AssetType, Texture2D>::value)
				{
					rawAsset = LoadTexture2D(assetName, filepath);
				}
				else if constexpr (std::is_same<AssetType, ShaderProgram>::value)
				{
					rawAsset = LoadShaderProgram(assetName, filepath);
				}
				else
				{
					AR_CORE_WARN("Unsupported asset type!");
					throw std::runtime_error("Unsupported asset type!");
				}

				asset->SetAsset(rawAsset);
				asset->SetState(AssetState::Loaded);
			}
			catch (std::exception& e)
			{
				asset->SetState(AssetState::Failed);
				eventMessage = e.what();
			}

			// Dispatch event and notify listeners
			DispatchAssetEvent(asset, eventMessage);

			// Execute callback
			if (callback)
				QueueCallback([callback, asset]() { callback(asset); });
		});
	}

	template <>
	void AssetManager::LoadAsset<ShaderProgram>(const std::string& name, const std::vector<Ref<Asset>>& shaders, const AssetCallbackFn callback)
	{
		// Prepare shaders & dependencies
		std::vector<Ref<Shader>> tempShaders;
		std::vector<uint32_t> tempDependencies;
		for (auto shader : shaders)
		{
			tempShaders.push_back(shader->GetAsset<Shader>());
			tempDependencies.push_back(shader->GetAssetId());
		}

		Ref<Asset> asset = StageAsset<ShaderProgram>(name, "", tempDependencies);

		// Check if asset is already loaded
		if (asset->GetState() == AssetState::Loaded)
		{
			if (callback)
				QueueCallback([callback, asset]() { callback(asset); });
			return;
		}

		// Load asset
		ThreadPool::SubmitTask([asset, tempShaders, callback]() {
			Ref<ShaderProgram> rawAsset = nullptr;
			std::string eventMessage;
			std::string assetName = asset->GetName();
			const uint32_t currentId = asset->GetAssetId();
			try
			{
				asset->SetState(AssetState::Loading);
				DispatchAssetEvent(asset);
				rawAsset = ShaderProgram::Create(assetName, tempShaders);
				asset->SetAsset(rawAsset);
				asset->SetState(AssetState::Loaded);
			}
			catch (const std::exception& e)
			{
				asset->SetState(AssetState::Failed);
				eventMessage = e.what();
			}

			// Dispatch event and notify listeners
			DispatchAssetEvent(asset, eventMessage);

			// Execute callback
			if (callback)
				QueueCallback([callback, asset]() { callback(asset); });
		});
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

	void AssetManager::DispatchAssetEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Ref<Event> event;
		switch (asset->GetState())
		{
		case AssetState::None: return;
		case AssetState::Staged: {
			AssetStagedEvent tempEvent(asset, message);
			EventQueue::Dispatch<AssetStagedEvent>(tempEvent);
			event = CreateRef<AssetStagedEvent>(tempEvent);
			break;
		}
		case AssetState::Loading: {
			AssetLoadingEvent tempEvent(asset, message);
			EventQueue::Dispatch<AssetLoadingEvent>(tempEvent);
			event = CreateRef<AssetLoadingEvent>(tempEvent);
			break;
		}
		case AssetState::Loaded: {
			AssetLoadedEvent tempEvent(asset, message);
			EventQueue::Dispatch<AssetLoadedEvent>(tempEvent);
			event = CreateRef<AssetLoadedEvent>(tempEvent);
			break;
		}
		case AssetState::Failed: {
			AssetFailedEvent tempEvent(asset, message);
			EventQueue::Dispatch<AssetFailedEvent>(tempEvent);
			event = CreateRef<AssetFailedEvent>(tempEvent);
			break;
		}
		default: return;
		}

		// Dispatch event to listeners
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);

			// Queue file-specific listeners
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

	Ref<VertexShader> AssetManager::LoadVertexShader(const std::string& name, const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Vertex Shader file: '" + filepath + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return VertexShader::Create(name, fileBuffer);
		return nullptr;
	}

	Ref<FragmentShader> AssetManager::LoadFragmentShader(const std::string& name, const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Fragment Shader file: '" + filepath + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return FragmentShader::Create(name, fileBuffer);
	}

	Ref<Texture2D> AssetManager::LoadTexture2D(const std::string& name, const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Texture2D file: '" + filepath + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		return Texture2D::Create(name, fileBuffer);
	}

	Ref<ShaderProgram> AssetManager::LoadShaderProgram(const std::string& name, const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load Shader Program file: '" + filepath + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		std::string shaderSource(static_cast<const char*>(fileBuffer.GetBuffer()), fileBuffer.GetSize());
		std::string vertexSource = "";
		std::string fragmentSource = "";

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

		if (vertexSource == "" || fragmentSource == "")
			throw std::runtime_error("One or more shaders are missing!");

		Ref<ShaderProgram> asset = ShaderProgram::Create(name, {
			VertexShader::Create(name + "_Vertex", vertexSource),
			FragmentShader::Create(name + "_Fragment", fragmentSource)
		});

		if (asset == nullptr)
			throw std::runtime_error("Some error occurred when created Shader Program!");
		else
			return asset;
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