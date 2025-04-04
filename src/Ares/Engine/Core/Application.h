/**
 * @file Application.h
 * @brief Defines the core Application class for the Ares engine.
 * 
 * @details This class serves as the central hub for running an application built on the Ares engine.
 * It manages the main loop, layers, event handling, and initialization of core systems like
 * rendering, thread pool, and asset management.
 */
#pragma once
#include <shared_mutex>

#include "Engine/Core/Flags.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Data/MemoryManager.h"
#include "Engine/Utility/Hash.h"

int EntryPoint(int argc, char** argv);

namespace Ares {

	class Event;
	class ImGuiContext;
	class ImGuiLayer;
	class Layer;
	class Window;
	class WindowCloseEvent;
	class WindowResizeEvent;
	enum class RenderAPI : uint8_t;

	namespace Internal {

		class System;

	}

	/**
	 * @struct ApplicationSettings
	 * @brief Configuration settings for the Application.
	 * 
	 * @details This struct provides options for setting up the application window,
	 * thread count, update rate, and other core properties.
	 * 
	 * @see Ares::WindowSettings::Flags
	 */
	struct ApplicationSettings
	{
		const char* Name = "Ares Engine";						///< The name of the application.
		uint32_t Width = 1280;									///< The width of the application window in pixels.
		uint32_t Height = 720;									///< The height of the application window in pixels.
		uint32_t UpdatesPerSecond = 120;						///< The update rate (ticks per second).
		uint8_t ThreadCount = 4;								///< Number of worker threads in the Application's ThreadPool.
		RenderAPI Renderer = static_cast<RenderAPI>(1);			///< The Renderer API that will be used.

		uint16_t WindowStyle = WindowSettings::DefaultWindow;	///< Style flags for the window.
		void* Icon = nullptr;									///< Pointer to the window icon resource.

		/**
		 * @brief Constructor for the ApplicationSettings struct.
		 * 
		 * @details Initializes the ApplicationSettings struct with default or specified values.
		 * The constructor allows you to configure the application's name, window dimensions,
		 * window style, and an optional icon.
		 * 
		 * @param name The name of the application (default: `"Ares Engine"`).
		 * @param width The width of the window (default: `1280`).
		 * @param height The height of the window (default: `720`).
		 * @param threadCount The number of worker threads in the Application's ThreadPool (default: `hardware_concurrency()`).
		 * @param windowStyle The window style, represented by a WindowSettings constant (default: `WindowSettings::DefaultWindow`).
		 * @param icon A pointer to the application icon (default: `nullptr`).
		 */
		ApplicationSettings(
			const char* name = "Ares Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			uint8_t threadCount = std::thread::hardware_concurrency(),
			uint16_t windowStyle = WindowSettings::DefaultWindow,
			void* icon = nullptr
		) : Name(name), Width(width), Height(height), ThreadCount(threadCount), WindowStyle(windowStyle), Icon(icon)
		{
		}
	};

	/**
	 * @class Application
	 * @brief Main class for the Ares engine application.
	 * 
	 * @details The Application class initializes and runs the main loop of an Ares-based application.
	 * It handles events, layers, and updates core engine systems like the renderer,
	 * asset manager, and input manager.
	 * 
	 * @see Ares::Systems
	 */
	class Application
	{
	public:
		/**
		 * @brief Constructor for the Application class.
		 * 
		 * @param settings The configuration settings for the application.
		 * @throws std::runtime_error If application instance already exists.
		 */
		Application(const ApplicationSettings& settings = ApplicationSettings());

		/**
		 * @brief Destructor for the Application class.
		 */
		virtual ~Application();

		/**
		 * @brief Adds a new layer to the application.
		 * 
		 * @param layer The layer to add.
		 */
		void PushLayer(Ref<Layer> layer);

		/**
		 * @brief Adds a new overlay layer to the application.
		 * 
		 * @param overlay The overlay layer to add.
		 */
		void PushOverlay(Ref<Layer> overlay);

		/**
		 * @brief Removes a layer from the application.
		 * 
		 * @param layer The layer to remove.
		 */
		void PopLayer(Ref<Layer> layer);

		/**
		 * @brief Removes an overlay from the application.
		 * 
		 * @param overlay The overlay to remove.
		 */
		void PopOverlay(Ref<Layer> overlay);

		/**
		 * @brief Registers a new system with the application.
		 *
		 * @details This template method registers a system of the type `SystemType` with the application.
		 * The system is constructed using the provided arguments (`args`).
		 *
		 * @tparam SystemType The type of system to register.
		 * @tparam Args The types of the arguments used to construct the system.
		 * @param args The arguments to forward to the system's constructor.
		 * @return `true` if the system was successfully registered; otherwise, `false`.
		 * 
		 * @see Ares::Systems
		 */
		template <typename SystemType, typename... Args>
		bool RegisterSystem(Args&&... args);

		/**
		 * @brief Retrieves a system of the specified type.
		 *
		 * @details This template method retrieves a system of the type `SystemType` that has been
		 * registered with the application. If the system is not found, it returns `nullptr`.
		 *
		 * @tparam SystemType The type of system to be retrieved.
		 * @return A pointer to the requested system, or `nullptr` if the system is not found.
		 */
		template <typename SystemType>
		SystemType* GetSystem();

		/**
		 * @brief Unregisters a system from the application.
		 *
		 * @details This template method unregisters a system of the type `SystemType` from the application.
		 * If the system is found, it is removed and destroyed.
		 *
		 * @tparam SystemType The type of the system to unregister.
		 */
		template <typename SystemType>
		void UnregisterSystem();

		/**
		 * @brief Retrieves the application window.
		 * 
		 * @return A reference to the Window object.
		 */
		Window& GetWindow() { return *m_Window; }

		/**
		 * @brief Retrieves the active Application instance.
		 * 
		 * @return A reference to the active Application object.
		 */
		static Application& Get() { return *s_Instance; }

		/**
		 * @brief Checks if the Application instance is valid.
		 * 
		 * @return `true` if the instance is valid; otherwise, `false`.
		 */
		static bool IsValid() { return s_Instance != nullptr; }

		/**
		 * @brief Retrieves the [MemoryManager](#Ares::Internal::MemoryManager) instance.
		 * 
		 * @details This method provides access to the internal memory manager, which is responsible
		 * for managing memory allocation and deallocation within the application.
		 * 
		 * @return A reference to the internal [MemoryManager](#Ares::Internal::MemoryManager) instance.
		 */
		Internal::MemoryManager& GetMemoryManager() { return m_MemoryManager; }

	private:
		/**
		 * @brief Runs the main application loop.
		 */
		void Run();

		void UpdateLoop();
		void RenderLoop();

		/**
		 * @brief Handles events dispatched to the application.
		 * 
		 * @param e Reference to the event being processed.
		 */
		void OnEvent(Event& e);

		/**
		 * @brief Handles the event triggered when the window is closed.
		 * 
		 * @param e The WindowCloseEvent triggering this callback.
		 * @return `true` if the event is handled; otherwise, `false`.
		 */
		bool OnWindowClose(WindowCloseEvent& e);

		/**
		 * @brief Handles the event triggered when the window is resized.
		 * 
		 * @param e The WindowResizeEvent triggering this callback.
		 * @return `true` if the event is handled; otherwise, `false`.
		 */
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		/**
		 * @typedef TimePoint
		 * @brief Alias for an `std::chrono::time_point`.
		 */
		using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

		Internal::MemoryManager m_MemoryManager;	///< Application Memory Manager.
		ApplicationSettings m_Settings;				///< Application configuration settings.
		Scope<LayerStack> m_LayerStack;				///< Stack of active layers in the application.
		Scope<Window> m_Window;						///< The main application window.
		Scope<ImGuiContext> m_ImGuiContext;			///< ImGui context for UI.
		Atomic<bool> m_Running = false;				///< Flag to indicate if the application is running.
		Atomic<bool> m_Minimized = false;					///< Flag to indicate if the application is minimized.
		TimePoint m_LastUpdateTime;					///< Last update time.
		TimePoint m_LastRenderTime;					///< Last render time.
		std::shared_mutex m_LayerStackMutex;		///< LayerStack mutex.
		std::shared_mutex m_Mutex;					///< Application mutex.

		HashMap<std::type_index, Scope<Internal::System>> m_Systems;	///< The application's systems.
		Vector<std::type_index> m_SystemOrder;							///< The order in which the application's system's `OnUpdate` methods are called.
		std::shared_mutex m_SystemMutex;								///< Systems mutex
	private:
		static Application* s_Instance;				///< Static reference to the active Application instance.

		friend int ::EntryPoint(int argc, char** argv);
	};

	/**
	 * @brief Function to create an application instance.
	 * Must be implemented in the client application.
	 * 
	 * @details
	 * **Example usage**:
	 * ```cpp
	 * // This MUST be implemented client-side.
	 * Ares::Application* Ares::CreateApplication()
	 * {
	 *    Ares::ApplicationSettings settings;
	 *    settings.WindowStyle = Ares::WindowSettings::DefaultWindow;
	 *    settings.ThreadCount = 4;
	 * 
	 *    Ares::Application* app = new Ares::Application(settings);
	 * 
	 *    // Create the layer using Ares::CreateRef after creating the application
	 *    // so that the layer is allocated using the Application's memory manager.
	 *    Ares::Ref<YourLayer> layer = Ares::CreateRef<YourLayer>();
	 *    app->PushLayer(layer);
	 * 
	 *    return app;
	 * }
	 * ```
	 * 
	 * @return A pointer to the created Application instance.
	 */
	Application* CreateApplication();

	template <typename SystemType, typename... Args>
	bool Application::RegisterSystem(Args&&... args)
	{
		std::unique_lock lock(m_SystemMutex);
		if (m_Systems[typeid(SystemType)] = SystemType::template Create(std::forward<Args>(args)...))
		{
			m_SystemOrder.emplace_back(typeid(SystemType));
			return true;
		}
		else
			return false;
	}

	template <typename SystemType>
	SystemType* Application::GetSystem()
	{
		std::shared_lock lock(m_SystemMutex);
		if (const auto& it = m_Systems.find(typeid(SystemType)); it != m_Systems.end())
			return static_cast<SystemType*>(it->second.get());
		else
			return nullptr;
	}

	template <typename SystemType>
	void Application::UnregisterSystem()
	{
		std::unique_lock lock(m_SystemMutex);
		if (const auto& it = m_Systems.find(typeid(SystemType)); it != m_Systems.end())
		{
			it->second.reset();
			m_Systems.erase(it);
		}
		if (const auto& orderIt = eastl::find(m_SystemOrder.begin(), m_SystemOrder.end(), typeid(SystemType)); orderIt != m_SystemOrder.end())
		{
			m_SystemOrder.erase(orderIt);
		}
	}

}