#include <arespch.h>
#include "Engine/Core/MainThreadQueue.h"

namespace Ares::Systems {

	Scope<MainThreadQueue> MainThreadQueue::Create()
	{
		return CreateScope<MainThreadQueue>();
	}

	MainThreadQueue::MainThreadQueue()
	{
	}

	void MainThreadQueue::SubmitTask(eastl::function<void()>&& task)
	{
		std::unique_lock lock(m_WriteMutex);
		m_WriteQueue.emplace(eastl::move(task));
	}

	void MainThreadQueue::OnUpdate(const Timestep& ts)
	{
		std::unique_lock lock1(m_ReadMutex);
		{
			std::unique_lock lock2(m_WriteMutex);
			eastl::swap(m_ReadQueue, m_WriteQueue);
		}

		while (!m_ReadQueue.empty())
		{
			auto task = eastl::move(m_ReadQueue.front());
			m_ReadQueue.pop();
			task();
		}
	}

}