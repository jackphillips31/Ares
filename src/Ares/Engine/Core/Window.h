/**
 * @file Window.h
 * @brief contains the declaration of the Window interface and associated data structures.
 * 
 * @details This file defines the Window interface that is used to represent a desktop system window,
 * along with the WindowProps structure for configuring window properties. The interface includes
 * various methods for updating, resizing, and handling window events. It also provides mechanisms
 * for controlling window settings such as V-Sync and position.
 */
#pragma once
#include <glm/vec2.hpp>

#include "Engine/Core/Flags.h"

namespace Ares {

	class Event;
	class GraphicsContext;

	/**
	 * @struct WindowProps
	 * @brief A structure to store properties for creating a window.
	 * 
	 * @details This structure contains the properties required to create a window, including title,
	 * size, position, flags, and the window icon.
	 */
	struct WindowProps
	{
		std::string Title;		///< The window's title.
		uint32_t Width;			///< The window's width.
		uint32_t Height;		///< The window's height.
		int32_t XPos;			///< The window's X position on screen.
		int32_t YPos;			///< The window's Y position on screen.
		uint16_t Flags;			///< The window's settings and flags. @see WindowSettings::Flags
		void* Icon;				///< Pointer to the window's icon.

		/**
		 * @brief Constructs a WindowProps object with default values.
		 * 
		 * @details Initializes the window properties with default values or custom values if provided.
		 * @param title The title of the window. Defaults to `Ares Engine`.
		 * @param width The width of the window. Defaults to `1280`.
		 * @param height The height of the window. Defaults to `720`.
		 * @param xpos The X position of the window on screen. Defaults to `200`.
		 * @param ypos The Y position of the window on screen. Defaults to `200`.
		 * @param flags The window settings flags. Defaults to WindowSettings::DefaultWindow.
		 * @param icon A pointer to the window's icon. Defaults to `nullptr`
		 */
		WindowProps(const std::string& title = "Ares Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			int32_t xpos = 200,
			int32_t ypos = 200,
			uint16_t flags = WindowSettings::DefaultWindow,
			void* icon = nullptr)
			: Title(title), Width(width), Height(height), XPos(xpos), YPos(ypos), Flags(flags), Icon(icon)
		{
		}
	};

	/**
	 * @class Window
	 * @brief Interface representing a desktop system-based window.
	 * 
	 * @details The Window interface defines essential methods for interacting with and manipulating
	 * a system window, such as updating, resizing, and handling events. The window can also
	 * be configured with different settings, such as V-Sync and position.
	 */
	class Window
	{
	public:
		/**
		 * @typedef EventCallbackFn
		 * @brief Alias for a function type that takes an Event reference and returns void.
		 */
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		/**
		 * @brief Updates the window.
		 * 
		 * @details This method is typically called every frame to update the window state, handle input,
		 * and process events.
		 */
		virtual void OnUpdate() = 0;

		/**
		 * @brief Swaps the buffers of the window.
		 * 
		 * @details This method is called to swap the front and back buffers of the window, typically
		 * to display the rendered content on screen.
		 */
		virtual void SwapBuffers() = 0;

		/**
		 * @brief Gets the width of the window.
		 * 
		 * @return The width of the window in pixels.
		 */
		virtual uint32_t GetWidth() const = 0;

		/**
		 * @brief Gets the height of the window.
		 *
		 * @return The height of the window in pixels.
		 */
		virtual uint32_t GetHeight() const = 0;

		/**
		 * @brief Gets the dimensions of the window.
		 * 
		 * @return The dimensions of the window as a `glm::uvec2` (width, height).
		 */
		virtual glm::uvec2 GetWindowDimensions() const = 0;

		/**
		 * @brief Gets the client width of the window.
		 * 
		 * @return The client width (excluding window borders) in pixels.
		 */
		virtual uint32_t GetClientWidth() const = 0;

		/**
		 * @brief Gets the client height of the window.
		 * 
		 * @return The client height (excluding window borders) in pixels.
		 */
		virtual uint32_t GetClientHeight() const = 0;

		/**
		 * @brief Gets the client dimensions for the window.
		 * 
		 * @return The client dimensions (excluding window borders) as a `glm::uvec2` (width, height).
		 */
		virtual glm::uvec2 GetClientDimensions() const = 0;

		/**
		 * @brief Gets the position of the window.
		 * 
		 * @return The position of the window as a `glm::ivec2` (x, y) on the screen.
		 */
		virtual glm::ivec2 GetWindowPos() const = 0;

		/**
		 * @brief Gets the client position of the window.
		 * 
		 * @return The position of the client area (excluding borders) as a `glm::ivec2` (x, y) on the screen.
		 */
		virtual glm::ivec2 GetClientPos() const = 0;

		/**
		 * @brief Gets the settings or flags for the window.
		 * 
		 * @return The window settings flags as uint16_t
		 * 
		 * @see WindowSettings::Flags
		 */
		virtual uint16_t GetWindowSettings() const = 0;

		// Window attributes

		/**
		 * @brief Enables or disables V-Sync.
		 * 
		 * @param enabled If true, V-Sycn will be enabled; otherwise, it will be disabled.
		 */
		virtual void SetVSync(bool enabled) = 0;

		/**
		 * @brief Checks if V-Sync is enabled.
		 * 
		 * @returns `true` if V-Sync is enabled, otherwise `false`.
		 */
		virtual bool IsVSync() const = 0;

		/**
		 * @brief Sets the position of the window.
		 * 
		 * @param x The new X position of the window.
		 * @param y The new Y position of the window.
		 */
		virtual void SetWindowPosition(int32_t x, int32_t y) = 0;

		/**
		 * @brief Sets the size of the window.
		 * 
		 * @param width The new width of the window.
		 * @param height The new height of the window.
		 */
		virtual void SetWindowSize(uint32_t width, uint32_t height) = 0;

		/**
		 * @brief Sets the client size of the window.
		 * 
		 * @param width The new client width of the window (excluding borders).
		 * @param height The new client height of the window (excluding borders).
		 */
		virtual void SetClientSize(uint32_t width, uint32_t height) = 0;

		/**
		 * @brief Sets both the size and position of the window.
		 * 
		 * @param x The new X position of the window.
		 * @param y The new Y position of the window.
		 * @param width The new width of the window.
		 * @param height The new height of the window.
		 */
		virtual void SetWindowSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;

		/**
		 * @brief Sets both the client size and position of the window.
		 * 
		 * @param x The new X position of the client area.
		 * @param y The new Y position of the client area.
		 * @param width The new client width of the window (excluding borders).
		 * @param height The new client height of the window (excluding borders).
		 */
		virtual void SetClientSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;

		/**
		 * @brief Sets the window settings or flags.
		 * 
		 * @param flags The new window settings flags.
		 * 
		 * @see WindowSettings::Flags
		 */
		virtual void SetWindowSettings(uint16_t flags) = 0;

		/**
		 * @brief Gets a pointer to the native window.
		 * 
		 * @return A pointer to the native window.
		 */
		virtual void* GetNativeWindow() const = 0;

		/**
		 * @brief Gets the graphics context associated with the window.
		 * 
		 * @return A pointer to the graphics context.
		 */
		virtual GraphicsContext* GetGraphicsContext() const = 0;

		/**
		 * @brief Creates a new window.
		 * 
		 * @param props The window properties to initialize the window with.
		 * 
		 * @return A Scope<Window> to the newly created window.
		 */
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}