/**
 * @file Layer.h
 * @brief Defines the Layer class for managing application behavior and rendering.
 * 
 * @details Layers are used to encapsulate different parts of the application, such as
 * game logic, UI, or rendering. They are managed by the LayerStack in the Application class.
 */
#pragma once

namespace Ares {

	class Event;
	class Timestep;

	/**
	 * @class Layer
	 * @brief Represents a layer in the Application
	 * 
	 * @details Layers are modular components of the application, enabling separation of concerns
	 * such as game logic, rendering, and user interface. Layers can be added or removed
	 * dynamically during runtime.
	 */
	class Layer
	{
	public:
		/**
		 * @brief Constructs a Layer with an optional debug name.
		 * @param name The name of the layer, used for debugging purposes.
		 */
		Layer(const std::string& name = "Layer");

		/**
		 * @brief Virtual destructor for a layer.
		 */
		virtual ~Layer();

		/**
		 * @brief Called when the layer is attached to the application.
		 */
		virtual void OnAttach() {}

		/**
		 * @brief Called when the layer is detached from the application.
		 */
		virtual void OnDetach() {}

		/**
		 * @brief Called during the application update cycle.
		 * @param ts The timestep representing the delta time since the last update.
		 */
		virtual void OnUpdate(const Timestep& ts) {}

		/**
		 * @brief Called during the application's render cycle.
		 */
		virtual void OnRender() {}

		/**
		 * @brief Called during the ImGui rendering cycle.
		 */
		virtual void OnImGuiRender() {}

		/**
		 * @brief Handles events specific to this layer.
		 * @param event The event to be handled.
		 */
		virtual void OnEvent(Event& event) {}

		/**
		 * @brief Retrieves the debug name of the layer.
		 * @return The name of the layer.
		 */
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;	///< Name of the layer for debugging purposes.
	};

}