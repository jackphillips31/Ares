/**
 * @file ThreadPool.h
 * @brief Defines the ThreadPool class for managing multi-threaded task execution.
 * 
 * @details The ThreadPool manages a fixed number of worker threads to execute tasks concurrently.
 * Tasks can be submitted via the SubmitTask method, which returns a future for retrieving
 * the result of the task.
 */
#pragma once
#include <future>

namespace Ares {

	/**
	 * @class ThreadPool
	 * @brief A thread pool for managing concurrent task execution
	 * 
	 * @details The ThreadPool creates a pool of worker threads to process tasks submitted
	 * to its queue. It supports dynamic task submission and is designed to be
	 * initialized once per application.
	 */
	class ThreadPool
	{
	public:
		/**
		 * @brief Initializes the ThreadPool with a given number of threads. Called during application startup.
		 * @param threadCount The number of worker threads to create (default: hardware concurrency).
		 */
		static void Init(size_t threadCount = std::thread::hardware_concurrency());

		/**
		 * @brief Shuts down the ThreadPool, ensuring all threads complete their tasks. Called during application shutdown.
		 */
		static void Shutdown();

		/**
		 * @brief Submits a task to the ThreadPool for execution.
		 * @tparam Func The callable type of the task.
		 * @tparam Args The types of arguments to the task.
		 * @param func The callable task.
		 * @param args The arguments to the task.
		 * @return A future object for retrieving the result of the task.
		 * 
		 * Example usage:
		 * ```cpp
		 * auto result = ThreadPool::SubmitTask([](int a, int b) { return a + b }, 5, 3);
		 * int sum = result.get(); // sum = 8
		 * ```
		 */
		template<typename Func, typename... Args>
		static auto SubmitTask(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>;

	private:
		static std::vector<std::thread> s_Workers;				///< Vector of worker threads.
		static std::queue<std::function<void()>> s_TaskQueue;	///< Queue of tasks to be executed.
		static std::mutex s_QueueMutex;							///< Mutex for synchronizing task queue access.
		static std::mutex s_InitMutex;							///< Mutex for thread pool initialization/shutdown.
		static std::condition_variable s_Condition;				///< Condition variable for task synchronization.
		static std::atomic<bool> s_ShutdownRequested;			///< Flag to indicate shutdown.
		static bool s_IsInitialized;							///< Flag to indicate if the thread pool is initialized.
	};

	// Template method implementations

	template <typename Func, typename... Args>
	auto ThreadPool::SubmitTask(Func&& func, Args&&... args)->std::future<decltype(func(args...))>
	{
		using ReturnType = decltype(func(args...));

		auto task = CreateRef<std::packaged_task<ReturnType()>>(
			std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
		);

		std::future<ReturnType> result = task->get_future();

		if (!s_IsInitialized || s_Workers.empty())
		{
			// Direct execution when no threads are available
			(*task)();
		}
		else
		{
			{
				std::lock_guard lock(s_QueueMutex);
				if (s_ShutdownRequested)
				{
					AR_CORE_ASSERT(false, "ThreadPool is shutting down, cannot submit new task!");
				}
				s_TaskQueue.emplace([task]() { (*task)(); });
			}
			s_Condition.notify_one();
		}

		return result;
	}

}