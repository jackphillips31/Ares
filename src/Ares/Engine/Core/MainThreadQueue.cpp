#include <arespch.h>
#include "Engine/Core/MainThreadQueue.h"

namespace Ares {

	void MainThreadQueue::SubmitTask(std::function<void()>&& function)
	{
		std::lock_guard lock(s_WriteMutex);
		s_WriteQueue.emplace(std::move(function));
	}

	void MainThreadQueue::Init()
	{
		Shutdown();
	}

	void MainThreadQueue::Shutdown()
	{
		{
			std::lock_guard lock(s_ReadMutex);
			while (!s_ReadQueue.empty()) s_ReadQueue.pop();
		}
		{
			std::lock_guard lock(s_WriteMutex);
			while (!s_WriteQueue.empty()) s_WriteQueue.pop();
		}
	}

	void MainThreadQueue::OnUpdate()
	{
		SwapQueues();

		std::lock_guard lock(s_ReadMutex);
		while (!s_ReadQueue.empty())
		{
			std::function<void()> func = std::move(s_ReadQueue.front());
			s_ReadQueue.pop();
			func();
		}
	}

	void MainThreadQueue::SwapQueues()
	{
		std::lock_guard lock1(s_WriteMutex);
		std::lock_guard lock2(s_ReadMutex);

		std::swap(s_WriteQueue, s_ReadQueue);
	}

}