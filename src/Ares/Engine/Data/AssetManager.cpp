#include <arespch.h>

#include "Engine/Core/ThreadPool.h"
#include "Engine/Data/FileIO.h"
#include "Engine/Utility/File.h"

#include "Engine/Data/AssetManager.h"

namespace Ares {

	std::unordered_map<std::type_index, std::unordered_map<uint32_t, Ref<AssetInfo>>> AssetManager::s_AssetCache;
	std::mutex AssetManager::s_CacheMutex;
	std::atomic<uint32_t> AssetManager::s_NextAssetId{ 1 };

	std::queue<std::function<void()>> AssetManager::s_CallbackQueue;
	std::mutex AssetManager::s_CallbackQueueMutex;

	std::unordered_map<std::string, std::vector<std::function<void(AssetLoadedEvent&)>>> AssetManager::s_Listeners;
	std::vector<std::function<void(AssetLoadedEvent&)>> AssetManager::s_GlobalListeners;
	std::mutex AssetManager::s_ListenerMutex;

	std::queue<std::function<void()>> AssetManager::s_ListenerCallbackQueue;
	std::mutex AssetManager::s_ListenerCallbackQueueMutex;

	// Explicit instantiations for LoadAsset types
	template void AssetManager::LoadAsset<VertexShader>(const std::string& name, const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback);
	template void AssetManager::LoadAsset<FragmentShader>(const std::string& name, const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback);
	template void AssetManager::LoadAsset<Texture2D>(const std::string& name, const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback);

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

	template <typename T>
	void AssetManager::LoadAsset(const std::string& name, const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback)
	{
		ThreadPool::SubmitTask([name, filepath, callback]()
		{
			Ref<T> asset = nullptr;
			bool success = false;
			std::string message;
			uint32_t currentId = 0;

			// Check cache
			{
				std::lock_guard<std::mutex> lock(s_CacheMutex);
				auto it = s_AssetCache.find(typeid(T));
				if (it != s_AssetCache.end())
				{
					auto& typeMap = it->second;
					for (const auto& typeIndex : typeMap)
					{
						if (typeIndex.second->Name == name)
						{
							// Asset exists - Get asset and call callback
							AssetLoadedEvent event(typeIndex.second, message);
							if (callback)
								QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });

							return;
						}
					}
				}
			}

			// Asset not in cache - Try loading asset
			try
			{
				if constexpr (std::is_same<T, VertexShader>::value)
				{
					asset = LoadVertexShader(name, filepath);
				}
				else if constexpr (std::is_same<T, FragmentShader>::value)
				{
					asset = LoadFragmentShader(name, filepath);
				}
				else if constexpr (std::is_same<T, Texture2D>::value)
				{
					asset = LoadTexture2D(name, filepath);
				}
				else
				{
					AR_CORE_WARN("Unsupported asset type!");
					throw std::runtime_error("Unsupported asset type!");
				}

				success = true;
				currentId = s_NextAssetId++;
				asset->SetAssetId(currentId);

				// Add to cache
				{
					std::lock_guard<std::mutex> lock(s_CacheMutex);
					s_AssetCache[typeid(T)][currentId] = CreateRef<AssetInfo>(asset, typeid(T).name(), AssetState::Loaded, filepath);
				}
			}
			catch (const std::exception& e)
			{
				message = e.what();
			}

			AssetLoadedEvent event(s_AssetCache[typeid(T)][currentId], message);

			// Notify all listeners
			NotifyListeners(name, event);

			// Dispatch event
			EventQueue::Dispatch<AssetLoadedEvent>(event);

			// Execute callback
			if (callback)
			{
				QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });
			}
		});
	}

	template<>
	void AssetManager::LoadAsset<ShaderProgram>(const std::string& name, const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback)
	{
		ThreadPool::SubmitTask([name, filepath, callback]()
		{
			Ref<ShaderProgram> asset = nullptr;
			bool success = false;
			std::string message;
			uint32_t currentId = 0;

			// Check cache
			{
				std::lock_guard<std::mutex> lock(s_CacheMutex);
				auto it = s_AssetCache.find(typeid(ShaderProgram));
				if (it != s_AssetCache.end())
				{
					auto& typeMap = it->second;
					for (const auto& typeIndex : typeMap)
					{
						if (typeIndex.second->Name == name)
						{
							// Asset exists - Get asset and call callback
							AssetLoadedEvent event(typeIndex.second, message);
							if (callback)
								QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });

							return;
						}
					}
				}
			}

			// Asset not in cache - Try loading asset
			try
			{
				FileBuffer shaderSourceFile = FileIO::LoadFile(filepath);
				std::string shaderSource(static_cast<const char*>(shaderSourceFile.GetBuffer()), shaderSourceFile.GetSize());
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

				asset = ShaderProgram::Create(name, {
					VertexShader::Create(name + "_Vertex", vertexSource),
					FragmentShader::Create(name + "_Fragment", fragmentSource)
				});

				if (asset == nullptr)
					throw std::runtime_error("Some error occurred when creating Shader Program!");

				success = true;
				currentId = s_NextAssetId++;
				asset->SetAssetId(currentId);

				// Add to cache
				{
					std::lock_guard<std::mutex> lock(s_CacheMutex);
					s_AssetCache[typeid(ShaderProgram)][currentId] = CreateRef<AssetInfo>(asset, typeid(ShaderProgram).name(), AssetState::Loaded, filepath);
				}
			}
			catch (const std::exception& e)
			{
				message = e.what();
			}

			AssetLoadedEvent event(s_AssetCache[typeid(ShaderProgram)][currentId], message);

			// Notify all listeners
			NotifyListeners(name, event);

			// Dispatch event
			EventQueue::Dispatch<AssetLoadedEvent>(event);

			// Execute callback
			if (callback)
			{
				QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });
			}
		});
	}

	template<>
	void AssetManager::LoadAsset<ShaderProgram>(const std::string& name, const std::vector<Ref<Shader>>& shaders, std::function<void(AssetLoadedEvent&)> callback)
	{
		ThreadPool::SubmitTask([name, shaders, callback]()
		{
			Ref<ShaderProgram> asset = nullptr;
			bool success = false;
			std::string message;
			uint32_t currentId = 0;

			// Check cache
			{
				std::lock_guard<std::mutex> lock(s_CacheMutex);
				auto it = s_AssetCache.find(typeid(ShaderProgram));
				if (it != s_AssetCache.end())
				{
					auto& typeMap = it->second;
					for (const auto& typeIndex : typeMap)
					{
						if (typeIndex.second->Name == name)
						{
							// Asset exists - Get asset and call callback
							AssetLoadedEvent event(typeIndex.second, message);
							if (callback)
								QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });

							return;
						}
					}
				}
			}

			// Asset not in cache - Try loading asset
			try
			{
				asset = ShaderProgram::Create(name, shaders);

				success = true;
				currentId = s_NextAssetId++;
				asset->SetAssetId(currentId);

				// Add to cache
				{
					std::lock_guard<std::mutex> lock(s_CacheMutex);
					s_AssetCache[typeid(ShaderProgram)][currentId] = CreateRef<AssetInfo>(asset, typeid(ShaderProgram).name(), AssetState::Loaded);
				}
			}
			catch (const std::exception& e)
			{
				message = e.what();
			}

			AssetLoadedEvent event(s_AssetCache[typeid(ShaderProgram)][currentId], message);

			// Notify all listeners
			NotifyListeners(name, event);

			// Dispatch event
			EventQueue::Dispatch<AssetLoadedEvent>(event);

			// Execute callback
			if (callback)
			{
				QueueCallback([callback, event]() { callback(const_cast<AssetLoadedEvent&>(event)); });
			}
		});
	}

	std::vector<Ref<AssetInfo>> AssetManager::GetCompleteList()
	{
		std::vector<Ref<AssetInfo>> result;
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, Ref<AssetInfo>>> assetCacheCopy;
		{
			std::lock_guard<std::mutex> lock(s_CacheMutex);
			assetCacheCopy = s_AssetCache;
		}
		for (auto& entry : assetCacheCopy)
		{
			for (auto& nextEntry : entry.second)
			{
				result.emplace_back(nextEntry.second);
			}
		}
		return result;
	}

	void AssetManager::AddListener(const std::string& name, std::function<void(AssetLoadedEvent&)> callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		s_Listeners[name].emplace_back(std::move(callback));
	}

	void AssetManager::AddGlobalListener(std::function<void(AssetLoadedEvent&)> callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		s_GlobalListeners.emplace_back(std::move(callback));
	}

	void AssetManager::OnUpdate()
	{
		ProcessCallbacks();
		ProcessListenerCallbacks();
	}

	void AssetManager::NotifyListeners(const std::string& name, AssetLoadedEvent event)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);

		// Queue file-specific listener callbacks
		if (s_Listeners.find(name) != s_Listeners.end())
		{
			for (auto& listener : s_Listeners[name])
			{
				QueueListenerCallback([listener, event]() { listener(const_cast<AssetLoadedEvent&>(event)); });
			}
		}

		// Queue global listener callbacks
		for (auto& globalListener : s_GlobalListeners)
		{
			QueueListenerCallback([globalListener, event]() { globalListener(const_cast<AssetLoadedEvent&>(event)); });
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
			std::string error = "Failed to load vertex shader file: '" + filepath + "'";
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
			std::string error = "Failed to load fragment shader file: '" + filepath + "'";
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
			std::string error = "Failed to load texture2D file: '" + filepath + "'";
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		return Texture2D::Create(name, fileBuffer);
	}

}