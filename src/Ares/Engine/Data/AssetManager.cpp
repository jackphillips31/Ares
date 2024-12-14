#include <arespch.h>

#include "Engine/Core/ThreadPool.h"
#include "Engine/Data/FileIO.h"
#include "Engine/Utility/File.h"

#include "Engine/Data/AssetManager.h"

namespace Ares {

	std::unordered_map<std::type_index, std::unordered_map<std::string, Ref<Asset>>> AssetManager::s_AssetCache;
	std::mutex AssetManager::s_CacheMutex;

	std::queue<std::function<void()>> AssetManager::s_CallbackQueue;
	std::mutex AssetManager::s_CallbackQueueMutex;

	std::unordered_map<std::string, std::vector<std::function<void(AssetLoadedEvent&)>>> AssetManager::s_Listeners;
	std::vector<std::function<void(AssetLoadedEvent&)>> AssetManager::s_GlobalListeners;
	std::mutex AssetManager::s_ListenerMutex;

	std::queue<std::function<void()>> AssetManager::s_ListenerCallbackQueue;
	std::mutex AssetManager::s_ListenerCallbackQueueMutex;

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

	void AssetManager::AddListener(const std::string& filepath, std::function<void(AssetLoadedEvent&)> callback)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		s_Listeners[filepath].emplace_back(std::move(callback));
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

	void AssetManager::NotifyListeners(const std::string& filepath, AssetLoadedEvent event)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);

		// Queue file-specific listener callbacks
		if (s_Listeners.find(filepath) != s_Listeners.end())
		{
			for (auto& listener : s_Listeners[filepath])
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

	Ref<Shader> AssetManager::LoadShader(const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load shader file: " + filepath;
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}
		return Shader::Create(
			Utility::File::GetFilenameNoExt(filepath),
			fileBuffer
		);
	}

	Ref<Texture2D> AssetManager::LoadTexture2D(const std::string& filepath)
	{
		FileBuffer fileBuffer = FileIO::LoadFile(filepath);
		if (!fileBuffer.GetBuffer() || fileBuffer.GetSize() == 0)
		{
			std::string error = "Failed to load texture2D file: " + filepath;
			AR_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		return Texture2D::Create(
			Utility::File::GetFilenameNoExt(filepath),
			fileBuffer
		);
	}

}