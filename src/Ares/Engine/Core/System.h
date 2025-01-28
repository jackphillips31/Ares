#pragma once

namespace Ares {

	class Timestep;

	namespace Internal {

		class System
		{
		public:
			virtual ~System() = default;

			virtual void OnUpdate(Timestep& ts) {};
		};

	}

	/**
	 * @namespace Ares::Systems
	 * 
	 * @brief The Systems namespace contains various systems
	 * responsible for managing and executing core functionalities in the Ares
	 * engine.
	 * 
	 * The systems in this namespace are designed to provide key engine features
	 * such as input handling, event handling, task management, asset management
	 * and more.
	 * 
	 * @details
	 * The following systems are included in the Systems namespace:
	 * @li Input: Provides input polling methods.
	 * @li ThreadPool: Provides a thread pool for executing tasks.
	 * 
	 * These systems are registered and created by the Application
	 * during Application construction.
	 */
	namespace System {}

}