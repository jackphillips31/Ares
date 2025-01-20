/**
 * @file MainThreadQueue.h
 * @brief Defines the MainThreadQueue class for managing main-thread-specific tasks.
 * 
 * @details The MainThreadQueue is used to enqueue tasks that must be executed on the main thread.
 * Tasks are submitted from other threads and processed during the main loop.
 */
#pragma once
#include <queue>

namespace Ares {

	class Application;

	/**
	 * @class MainThreadQueue
	 * @brief Handles tasks that must run on the main thread.
	 * 
	 * @details This class provides a thread-safe way to submit tasks to be executed on the main thread.
	 * Tasks are enqueued by other thread and processed in the application's main loop.
	 */
	class MainThreadQueue
	{
	public:
		MainThreadQueue() = delete;

		/**
		 * @brief Submits a task to the main thread queue.
		 * 
		 * @param function The task to be executed on the main thread.
		 */
		static void SubmitTask(std::function<void()>&& function);

	private:
		friend class Application;

		/**
		 * @brief Initializes the MainThreadQueue. Called during application startup.
		 */
		static void Init();

		/**
		 * @brief Shuts down the MainThreadQueue. Called during application shutdown.
		 */
		static void Shutdown();

		/**
		 * @brief Processes all tasks currently in the queue.
		 * 
		 * @details This method should be called during the application's main loop.
		 */
		static void OnUpdate();

		/**
		 * @brief Swaps the read a write queues for task processing.
		 */
		static void SwapQueues();

	private:
		static inline std::mutex s_ReadMutex;							///< Mutex for protecting the read queue.
		static inline std::mutex s_WriteMutex;							///< Mutex for protecting the write queue.
		static inline std::queue<std::function<void()>> s_ReadQueue;	///< Queue of tasks ready for execution.
		static inline std::queue<std::function<void()>> s_WriteQueue;	///< Queue of tasks waiting to be swapped.
	};

}