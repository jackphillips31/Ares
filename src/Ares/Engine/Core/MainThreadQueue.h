#pragma once
#include <queue>

namespace Ares {

	class Application;

	class MainThreadQueue
	{
	public:
		MainThreadQueue() = delete;

		static void SubmitTask(std::function<void()>&& function);

	private:
		friend class Application;
		static void Init();
		static void Shutdown();
		static void OnUpdate();
		static void SwapQueues();

	private:
		static inline std::mutex s_ReadMutex;
		static inline std::mutex s_WriteMutex;
		static inline std::queue<std::function<void()>> s_ReadQueue;
		static inline std::queue<std::function<void()>> s_WriteQueue;
	};

}