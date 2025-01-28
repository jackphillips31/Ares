#include <arespch.h>
#include "Engine/Core/ThreadPool.h"

#include "Engine/Core/Application.h"

namespace Ares::Systems {

	Scope<ThreadPool> ThreadPool::Create(size_t threadCount)
	{
		return Scope<ThreadPool>(new ThreadPool(threadCount));
	}

	ThreadPool::ThreadPool(size_t threadCount)
		: m_ShutdownRequested(false)
	{
		std::unique_lock lock(m_WorkerMutex);

		m_Workers.reserve(threadCount);

		for (size_t i = 0; i < threadCount; i++)
		{
			m_Workers.emplace_back([this, i]
				{
					while (true)
					{
						eastl::function<void()> task;
						{
							std::unique_lock lock(m_QueueMutex);
							m_Condition.wait(lock, [this]
								{
									return m_ShutdownRequested || !m_TaskQueue.empty();
								});

							if (m_ShutdownRequested && m_TaskQueue.empty())
								return;

							task = eastl::move(m_TaskQueue.front());
							m_TaskQueue.pop();
						}
						task();
					}
				});
		}
	}

	ThreadPool::~ThreadPool()
	{
		m_ShutdownRequested = true;
		m_Condition.notify_all();

		{
			std::unique_lock lock(m_WorkerMutex);

			for (std::thread& thread : m_Workers)
			{
				if (thread.joinable())
					thread.join();
			}

			m_Workers.clear();
		}

		{
			std::unique_lock lock(m_QueueMutex);

			while (!m_TaskQueue.empty())
				m_TaskQueue.pop();
		}
	}

}