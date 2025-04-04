#include <arespch.h>
#include "Engine/Core/MainThreadQueue.h"

namespace Ares::Systems {

	Scope<MainThreadQueue> MainThreadQueue::Create()
	{
		return CreateScope<MainThreadQueue>();
	}

	MainThreadQueue::MainThreadQueue()
	{
		AR_CORE_INFO("Initializing System: MainThreadQueue");
	}

	MainThreadQueue::~MainThreadQueue()
	{
		std::unique_lock lock1(m_ReadMutex, std::defer_lock);
		std::unique_lock lock2(m_WriteMutex, std::defer_lock);
		std::lock(lock1, lock2);
		while (!m_ReadQueue.empty())
			m_ReadQueue.pop();
		while (!m_WriteQueue.empty())
			m_WriteQueue.pop();
	}

	void MainThreadQueue::SubmitTask(Function<void()>&& task)
	{
		std::unique_lock lock(m_WriteMutex);
		m_IsDirty = true;
		m_WriteQueue.emplace(eastl::move(task));
	}

	void MainThreadQueue::OnRender()
	{
		if (m_IsDirty.load())
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

			m_IsDirty = false;
		}
	}

}