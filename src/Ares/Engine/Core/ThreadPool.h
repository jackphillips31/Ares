/**
 * @file ThreadPool.h
 * @brief Defines the ThreadPool class for managing multi-threaded task execution.
 * 
 * @details This header file defines the ThreadPool class, which manages a fixed number of
 * worker threads to execute tasks concurrently. Tasks can be submitted via the [SubmitTask](#ThreadPool::SubmitTask)
 * method, which returns a future for retrieving the result of the task.
 * 
 * **Example Usage with Application**:
 * ```cpp
 * #include <Ares.h>
 *
 * using namespace Ares;
 *
 * Systems::ThreadPool* threadPoolSys = Application::Get().GetSystem<Systems::ThreadPool>();
 *
 * auto result = threadPoolSys->SubmitTask([](int32_t a, int32_t b) { return a + b; }, 5, 6);
 *
 * int32_t resultInt = result.get(); // sum = 11
 * ```
 *
 * **Example Usage without Application**:
 * ```cpp
 * #include <Engine/Core/ThreadPool.h>
 *
 * using namespace Ares;
 *
 * Scope<ThreadPool> threadPool = Systems::ThreadPool::Create(4) // 4 for thread count
 *
 * threadPool->SubmitTask([]()
 * {
 *     // Do something
 * });
 * ```
 * @note While multiple ThreadPool instances can be created, it is recommended to use a
 * single instance for optimal management. Use [SubmitTask](#ThreadPool::SubmitTask) to add
 * tasks and retrieve results via `std::future`.
 */
#pragma once
#include "Engine/Core/System.h"
#include <future>
#include <shared_mutex>
#include <EASTL/vector.h>
#include <EASTL/atomic.h>
#include <EASTL/queue.h>

namespace Ares {

	class Application;

	namespace Internal {

		class AppAllocator;
		struct Deleter;

	}

	namespace Systems {

		/**
		 * @class ThreadPool
		 * @brief Manages a pool of worker threads for concurrent task execution.
		 *
		 * @details The ThreadPool creates and manages a fixed number of worker threads to process tasks
		 * submitted to its queue. Tasks are executed concurrently, and results can be retrieved asynchronously
		 * via `std::future`.
		 *
		 * **Example Usage with Application**:
		 * ```cpp
		 * #include <Ares.h>
		 *
		 * using namespace Ares;
		 *
		 * Systems::ThreadPool* threadPoolSys = Application::Get().GetSystem<Systems::ThreadPool>();
		 *
		 * auto result = threadPoolSys->SubmitTask([](int32_t a, int32_t b) { return a + b; }, 5, 6);
		 *
		 * int32_t resultInt = result.get(); // sum = 11
		 * ```
		 *
		 * **Example Usage without Application**:
		 * ```cpp
		 * #include <Engine/Core/ThreadPool.h>
		 * 
		 * using namespace Ares;
		 * 
		 * Systems::ThreadPool* threadPool = Systems::ThreadPool::Create(4) // 4 for thread count
		 * 
		 * threadPool->SubmitTask([]()
		 * {
		 *     // Do something
		 * });
		 * ```
		 * @note While multiple ThreadPool instances can be created, it is recommended to use a
		 * single instance for optimal management. Use [SubmitTask](#ThreadPool::SubmitTask) to add
		 * tasks and retrieve results via `std::future`.
		 */
		class ThreadPool : public Internal::System
		{
		public:
			~ThreadPool() override;

			/**
			 * @brief Submits a task to the ThreadPool for execution.
			 *
			 * @tparam Func The callable type of the task.
			 * @tparam Args The types of arguments to the task.
			 * @param func The callable task.
			 * @param args The arguments to the task.
			 * @return A future object for retrieving the result of the task.
			 *
			 * **Example usage**:
			 * ```cpp
			 * #include <Ares.h>
			 * 
			 * using namespace Ares;
			 * 
			 * Systems::ThreadPool* threadPool = Application::Get().GetSystem<Systems::ThreadPool>();
			 * auto result = threadPool->SubmitTask([](int a, int b) { return a + b }, 5, 3);
			 * 
			 * int sum = result.get(); // sum = 8
			 * ```
			 */
			template <typename Func, typename... Args>
			auto SubmitTask(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>;

			/**
			 * @brief Creates an instance of the ThreadPool class. Called during Application construction.
			 *
			 * @details This static method creates an instance of the ThreadPool class.
			 *
			 * @param threadCount The number of worker threads to create (default: hardware concurrency).
			 * @return A Scope to the created ThreadPool object.
			 */
			static Scope<ThreadPool> Create(size_t threadCount = std::thread::hardware_concurrency());

			template <typename DeleterType, typename AllocatorType>
			static Scope<ThreadPool, DeleterType> Create(const AllocatorType& alloc, size_t threadCount = std::thread::hardware_concurrency());

		private:
			ThreadPool(size_t threadCount);

		private:
			eastl::vector<std::thread> m_Workers;					///< Vector of worker threads.
			eastl::queue<eastl::function<void()>> m_TaskQueue;		///< Queue of tasks to be executed.
			std::mutex m_QueueMutex;								///< Mutex for synchronizing task queue access.
			std::shared_mutex m_WorkerMutex;						///< Mutex for synchronizing work done on worker.
			std::condition_variable m_Condition;					///< Condition variable for task synchronization.
			eastl::atomic<bool> m_ShutdownRequested;				///< Flag to indicate shutdown.
		};

		template <typename Func, typename... Args>
		auto ThreadPool::SubmitTask(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
		{
			using ReturnType = decltype(func(args...));

			auto task = CreateRef<std::packaged_task<ReturnType()>>(
				std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
			);

			std::future<ReturnType> result = task->get_future();

			std::shared_lock lock1(m_WorkerMutex);
			if (m_Workers.empty())
				(*task)();
			else
			{
				std::unique_lock lock2(m_QueueMutex);
				if (m_ShutdownRequested)
				{
					AR_CORE_ASSERT(false, "ThreadPool is shutting down, cannot submit new task!");
					return result;
				}
				m_TaskQueue.emplace([task]() { (*task)(); });
				m_Condition.notify_one();
			}

			return result;
		}

		template <typename DeleterType, typename AllocatorType>
		Scope<ThreadPool, DeleterType> ThreadPool::Create(const AllocatorType& alloc, size_t threadCount)
		{
			return Scope<ThreadPool, DeleterType>(new (alloc.allocate(sizeof(ThreadPool))) ThreadPool(threadCount), DeleterType(alloc));
		}

	}

}