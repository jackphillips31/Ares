#pragma once

namespace Ares {

	class ThreadPool
	{
	public:
		// Initialize the thread pool
		static void Init(size_t threadCount = std::thread::hardware_concurrency());

		// Shutdown the thread pool
		static void Shutdown();

		// Submit a task to the pool
		template<typename Func, typename... Args>
		static auto SubmitTask(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>;

	private:
		// Worker threads
		static std::vector<std::thread> s_Workers;

		// Task queue
		static std::queue<std::function<void()>> s_TaskQueue;

		// Mutex to protect shared state
		static std::mutex s_QueueMutex;
		static std::mutex s_InitMutex;
		static std::condition_variable s_Condition;

		// Flags for shutdown and initialization
		static std::atomic<bool> s_ShutdownRequested;
		static bool s_IsInitialized;
	};

	template <typename Func, typename... Args>
	auto ThreadPool::SubmitTask(Func&& func, Args&&... args)->std::future<decltype(func(args...))>
	{
		using ReturnType = decltype(func(args...));

		auto task = CreateRef<std::packaged_task<ReturnType()>>(
			std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
		);

		std::future<ReturnType> result = task->get_future();

		{
			std::lock_guard<std::mutex> lock(s_QueueMutex);
			if (s_ShutdownRequested)
			{
				AR_CORE_ASSERT(false, "ThreadPool is shutting down, cannot submit new task!");
			}
			s_TaskQueue.emplace([task]() { (*task)(); });
		}

		s_Condition.notify_one();
		return result;
	}

}