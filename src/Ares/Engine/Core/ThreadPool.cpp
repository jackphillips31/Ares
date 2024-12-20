#include <arespch.h>

#include "Engine/Core/Application.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Core/ThreadPool.h"

namespace Ares {

	std::vector<std::thread> ThreadPool::s_Workers;
	std::vector<Scope<SharedGraphicsContext>> ThreadPool::s_SharedContexts;
	std::queue<std::function<void()>> ThreadPool::s_TaskQueue;
	std::mutex ThreadPool::s_QueueMutex;
	std::mutex ThreadPool::s_InitMutex;
	std::condition_variable ThreadPool::s_Condition;
	std::atomic<bool> ThreadPool::s_ShutdownRequested = false;
	bool ThreadPool::s_IsInitialized = false;

	void ThreadPool::Init(size_t threadCount)
	{
		std::lock_guard<std::mutex> lock(s_InitMutex);
		if (s_IsInitialized)
			return;

		AR_CORE_INFO("Initializing ThreadPool");
		s_IsInitialized = true;

		s_SharedContexts = std::vector<Scope<SharedGraphicsContext>>(threadCount);

		s_ShutdownRequested = false;
		s_Workers.reserve(threadCount);

		Application& app = Application::Get();

		for (size_t i = 0; i < threadCount; i++)
		{
			s_SharedContexts[i] = SharedGraphicsContext::Create(app.GetWindow().GetGraphicsContext()->GetContextHandle());
			s_Workers.emplace_back([i] {
				SharedGraphicsContext& context = *s_SharedContexts[i];
				context.MakeCurrent();
				while (true)
				{
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(s_QueueMutex);
						s_Condition.wait(lock, [] {
							return s_ShutdownRequested || !s_TaskQueue.empty();
						});

						if (s_ShutdownRequested && s_TaskQueue.empty())
							return;

						task = std::move(s_TaskQueue.front());
						s_TaskQueue.pop();
					}
					task();
					RenderCommand::Finish();
				}
			});
		};
	}

	void ThreadPool::Shutdown()
	{
		std::lock_guard<std::mutex> lock(s_InitMutex);
		if (!s_IsInitialized)
			return;
		s_IsInitialized = false;

		{
			std::lock_guard<std::mutex> lock(s_QueueMutex);
			s_ShutdownRequested = true;
		}

		s_Condition.notify_all();
		for (std::thread& worker : s_Workers)
		{
			if (worker.joinable())
				worker.join();
		}

		s_Workers.clear();
		while (!s_TaskQueue.empty())
			s_TaskQueue.pop();
	}

}