/**
 * @file MainThreadQueue.h
 * @brief Defines a system for managing and executing tasks on the main thread.
 * 
 * @details This file contains the declaration of the MainThreadQueue class, which provides
 * a thread-safe mechanism for submitting tasks to be executed on the main thread.
 * It uses a double-buffered queue system to minimize contention between task submission
 * and execution.
 * 
 * **Example Usage with Application**:
 * ```cpp
 * #include <Ares.h>
 * 
 * using namespace Ares;
 * 
 * Systems::MainThreadQueue* mainThreadSys = Application::Get().GetSystem<Systems::MainThreadQueue>();
 * 
 * mainThreadSys->SubmitTask([]() {
 *     // Do something here
 * });
 * ```
 * 
 * **Example Usage without Application**:
 * ```cpp
 * #include <Ares.h>
 * 
 * using namespace Ares;
 * 
 * Scope<Systems::MainThreadQueue> mainThreadSys = Systems::MainThreadQueue::Create();
 * 
 * mainThreadSys->SubmitTask([]() {
 *     // Do something
 * });
 * 
 * while (true)
 * {
 *     mainThreadSys->OnUpdate();
 * }
 * ```
 */
#pragma once
#include "Engine/Core/System.h"
#include "Engine/Containers/Function.h"

namespace Ares::Systems {

	/**
	 * @class MainThreadQueue
	 * @brief A system for executing tasks on the main thread in a thread-safe manner.
	 * 
	 * @details The MainThreadQueue provides functionality to submit tasks from any thread
	 * and have them executed safely on the main thread during the update cycle.
	 * It implements a double-buffered queue system to minimize locking contention.
	 * 
	 * **Example Usage with Application**:
	 * ```cpp
	 * #include <Ares.h>
	 *
	 * using namespace Ares;
	 *
	 * Systems::MainThreadQueue* mainThreadSys = Application::Get().GetSystem<Systems::MainThreadQueue>();
	 *
	 * mainThreadSys->SubmitTask([]() {
	 *     // Do something here
	 * });
	 * ```
	 *
	 * **Example Usage without Application**:
	 * ```cpp
	 * #include <Ares.h>
	 *
	 * using namespace Ares;
	 *
	 * Scope<Systems::MainThreadQueue> mainThreadSys = Systems::MainThreadQueue::Create();
	 *
	 * mainThreadSys->SubmitTask([]() {
	 *     // Do something
	 * });
	 *
	 * while (true)
	 * {
	 *     mainThreadSys->OnRender();
	 * }
	 * ```
	 */
	class MainThreadQueue : public Internal::System
	{
	private:
		/**
		 * @brief Private constructor to enforce creation via [Create()](#Ares::Systems::MainThreadQueue::Create) method.
		 */
		MainThreadQueue();

	public:
		/**
		 * @brief Default destructor.
		 */
		~MainThreadQueue();
		
		/**
		 * @brief Submits a task to be executed on the main thread.
		 * 
		 * @details This method is thread-safe and can be called from any thread.
		 * The task will be executed during the next render cycle on the main thread.
		 * 
		 * @param task A function object representing the task to execute.
		 */
		void SubmitTask(Function<void()>&& task);

		/**
		 * @brief Executes all the pending tasks in the queue.
		 * 
		 * @details This method is called automatically by the engine during the main render loop.
		 * It swaps the read and write queues and executes all tasks in the read queue.
		 * 
		 * @note This method is overriding the `OnRender` method rather than the `OnUpdate` method
		 * because the [Application](#Ares::Application) executes the MainThreadQueue tasks during
		 * the render loop.
		 */
		void OnRender() override;

		/**
		 * @brief Creates a new instance of MainThreadQueue.
		 * 
		 * @details This is the preferred way to create a MainThreadQueue instance.
		 * 
		 * @return A Scope<MainThreadQueue> object.
		 */
		static Scope<MainThreadQueue> Create();

	private:
		// Friend declaration for CreateScope to allow private constructor access
		template <typename ObjectType, typename... Args>
		friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

	private:
		Atomic<bool> m_IsDirty;						///< Bool to tell whether there are tasks to run.
		Queue<Function<void()>> m_ReadQueue;		///< Queue for reading tasks during update.
		Queue<Function<void()>> m_WriteQueue;		///< Queue for writing new tasks.
		mutable std::shared_mutex m_ReadMutex;		///< Mutex for protecting read queue access.
		mutable std::shared_mutex m_WriteMutex;		///< Mutex for protection write queue access.
	};

}