/**
 * @file System.h
 * @brief Defines the base class for all systems of the Ares Application.
 * 
 * @details The System base class is an abstract class that defines the common
 * interface for all systems in the Ares engine. Derived systems optionally implement
 * the `OnUpdate` method to define their behavior during each frame update.
 * 
 * This file also includes documentation for the [Internal](#Ares::Internal) namespace
 * as well as documentation for the [Systems](#Ares::Systems) namespace.
 */
#pragma once

namespace Ares {

	class Timestep;

	/**
	 * @namespace Ares::Internal
	 * @brief Internal components of the Ares engine.
	 * 
	 * @details The Ares::Internal namespace contains implementation details and internal utilities
	 * that are not intended for direct use by end-users. These components are subject to change
	 * and should not be relied upon in external code.
	 */
	namespace Internal {

		/**
		 * @class System
		 * @brief Base class for all systems in the Ares engine.
		 * 
		 * @details The System class is an abstract base class that defines the common interface
		 * for all systems in the Ares engine, such as [Input](#Ares::Systems::Input), [ThreadPool](#Ares::Systems::ThreadPool),
		 * [AssetManager](#Ares::Systems::AssetManager), [EventQueue](#Ares::Systems::EventQueue), and [Renderer](#Ares::Systems::Renderer).
		 * Derived systems optionally implement the `OnUpdate` method to define their behavior during each frame update.
		 * 
		 * @note This class is purely virtual and cannot be instantiated directly.
		 */
		class System
		{
		public:
			/**
			 * @brief Virtual destructor.
			 * 
			 * @details Ensures proper cleanup of derived system objects.
			 */
			virtual ~System() = default;

			/**
			 * @brief Called during each logic update.
			 * 
			 * @details This method is optionally implemented by derived classes to define
			 * the system's behavior during each logic update. The [Timestep](#Ares::Timestep) parameter provides
			 * information about the time elapsed since the last update.
			 * 
			 * @param ts The [Timestep](#Ares::Timestep) representing the time elapsed since the last update.
			 */
			virtual void OnUpdate(const Timestep& ts) {};

			/**
			 * @brief Called during each frame update.
			 * 
			 * @details This method is optionally implemented by derived classes to define
			 * the system's behavior during each render.
			 */
			virtual void OnRender() {};
		};

	}

	/**
	 * @namespace Ares::Systems
	 * @brief [System](#Ares::Internal::System) components of the Ares engine.
	 * 
	 * @details The systems in this namespace are designed to provide key engine features
	 * such as input handling, event handling, task management, asset management
	 * and more.
	 * 
	 * The following systems are included in the Systems namespace:
	 * @li Input: Provides input polling methods.
	 * @li ThreadPool: Provides a thread pool for executing tasks.
	 * @li AssetManager: Provides a system for managing assets such as [Textures](#Ares::Texture), individual [Shaders](#Ares::Shader),
	 * [Shader Programs](#Ares::ShaderProgram), and [MeshData](#Ares::MeshData).
	 * @li EventQueue: Provides a system for handling events, dispatching events to the application, and dispatching events to listeners.
	 * @li Renderer: Provides methods for executing render calls.
	 * 
	 * These systems are registered and created by the Application
	 * during Application construction.
	 */
	namespace Systems {}

}