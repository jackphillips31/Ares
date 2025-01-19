/**
 * @file Application.h
 * @brief Defines the core Application class for the Ares Engine.
 * 
 * @details This class serves as the central hub for running an application built on the Ares Engine.
 * It manages the main loop, layers, event handling, and initialization of core systems like
 * rendering, thread pool, and asset management.
 */
#pragma once
#include "Engine/Core/Flags.h"
#include "Engine/Core/LayerStack.h"

int EntryPoint(int argc, char** argv);

namespace Ares {

	class Event;
	class EventQueue;
	class ImGuiLayer;
	class Layer;
	class Window;
	class WindowCloseEvent;
	class WindowResizeEvent;

	/**
	 * @struct ApplicationSettings
	 * @brief Configuration settings for the Application.
	 * 
	 * @details This struct provides options for setting up the application window,
	 * thread count, update rate, and other core properties.
	 */
	struct ApplicationSettings
	{
		std::string Name = "Ares Engine";						///< The name of the application.
		uint32_t Width = 1280;									///< The width of the application window in pixels.
		uint32_t Height = 720;									///< The height of the application window in pixels.
		uint32_t UpdatesPerSecond = 120;						///< The update rate (ticks per second).
		uint8_t ThreadCount = 4;								///< Number of worker threads in the ThreadPool.

		uint16_t WindowStyle = WindowSettings::DefaultWindow;	///< Style flags for the window.
		void* Icon = nullptr;									///< Pointer to the window icon resource.

		/**
		 * @brief Constructor for the ApplicationSettings struct.
		 * 
		 * @details Initializes the ApplicationSettings struct with default or specified values.
		 * The constructor allows you to configure the application's name, window dimensions,
		 * window style, and an optional icon.
		 * 
		 * @param name The name of the application (default: "Ares Engine").
		 * @param width The width of the window (default: 1280).
		 * @param height The height of the window (default: 720).
		 * @param windowStyle The window style, represented by a WindowSettings constant (default: WindowSettings::DefaultWindow).
		 * @param icon A pointer to the application icon (default: `nullptr`).
		 */
		ApplicationSettings(
			const std::string& name = "Ares Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			uint16_t windowStyle = WindowSettings::DefaultWindow,
			void* icon = nullptr
		) : Name(name), Width(width), Height(height), WindowStyle(windowStyle), Icon(icon)
		{
		}
	};

	/**
	 * @class Application
	 * @brief Main class for the Ares Engine application.
	 * 
	 * @details The Application class initializes and runs the main loop of an Ares-based application.
	 * It handles events, layers, and updates core engine systems like the renderer,
	 * asset manager, and input manager.
	 */
	class Application
	{
	public:
		/**
		 * @brief Constructor for the Application class.
		 * @param settings The configuration settings for the application.
		 */
		Application(const ApplicationSettings& settings = ApplicationSettings());

		/**
		 * @brief Destructor for the Application class.
		 */
		virtual ~Application();

		/**
		 * @brief Adds a new layer to the application.
		 * @param layer The layer to add.
		 */
		void PushLayer(Ref<Layer> layer);

		/**
		 * @brief Adds a new overlay layer to the application.
		 * @param overlay The overlay layer to add.
		 */
		void PushOverlay(Ref<Layer> overlay);

		/**
		 * @brief Retrieves the application window.
		 * @return A reference to the Window object.
		 */
		inline Window& GetWindow() { return *m_Window; }

		/**
		 * @brief Retrieves the active Application instance.
		 * @return A reference to the active Application object.
		 */
		inline static Application& Get() { return *s_Instance; }

	private:
		/**
		 * @brief Runs the main application loop.
		 */
		void Run();

		/**
		 * @brief Handles events dispatched to the application.
		 * @param e Reference to the event being processed.
		 */
		void OnEvent(Event& e);

		/**
		 * @brief Handles the event triggered when the window is closed.
		 * @param e The WindowCloseEvent triggering this callback.
		 * @return true if the event is handled, false otherwise.
		 */
		bool OnWindowClose(WindowCloseEvent& e);

		/**
		 * @brief Handles the event triggered when the window is resized.
		 * @param e The WindowResizeEvent triggering this callback.
		 * @return true if the event is handled, false otherwise.
		 */
		bool OnWindowResize(WindowResizeEvent& e);
	
	private:
		ApplicationSettings m_Settings;		// Application configuration settings.
		Scope<Window> m_Window;				// The main application window.
		Ref<ImGuiLayer> m_ImGuiLayer;		// ImGui overlay for debugging/UI.
		bool m_Running = true;				// Flag to indicate if the application is running.
		bool m_Minimized = false;			// Flag to indicate if the application is minimized.
		LayerStack m_LayerStack;			// Stack of active layers in the application.
		double m_LastFrameTime = 0.0f;		// Timestamp of the last frame update.

	private:
		static Application* s_Instance;		// Static reference to the active Application instance.

		friend int ::EntryPoint(int argc, char** argv);
		friend class EventQueue;
	};

	/**
	 * @brief Function to create an application instance.
	 * Must be implemented in the client application.
	 * @return A pointer to the created Application instance.
	 * @details
	 * Example usage:
	 * ```cpp
	 * // This MUST be implemented client-side.
	 * Ares::Application* Ares::CreateApplication()
	 * {
	 *    Ares::ApplicationSettings settings;
	 *    settings.WindowStyle = Ares::WindowSettings::DefaultWindow;
	 *    settings.ThreadCount = 4;
	 * 
	 *    std::shared_ptr<YourLayer> layer = std::make_shared<YourLayer>();
	 * 
	 *    Ares::Application* app = new Ares::Application(settings);
	 *    app->PushLayer(layer);
	 * 
	 *    return app;
	 * }
	 * ```
	 */
	Application* CreateApplication();

}