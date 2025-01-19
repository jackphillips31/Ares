/**
 * @file Input.h
 * @brief Interface for handling user input (keyboard and mouse).
 * 
 * @details This header file defines the Input class, which provides platform-independent
 * methods for polling input states, such as checking if keys or mouse buttons are pressed,
 * and getting the position of the mouse. The actual implementation of these methods is platform-specific
 * and is provided by derived classes.
 * 
 * Example usage:
 * ```cpp
 * if (Input::IsKeyPressed(KeyCode::W)) {
 *    // Do something when 'W' key is pressed
 * }
 * glm::ivec2 mousePos = Input::GetMousePosition();
 * ```
 * 
 * @note The Input class is a base class. To use it, a platform-specific subclass
 * must be created that implements the virtual methods.
 * 
 * @see KeyCode, MouseCode
 */
#pragma once
#include <glm/vec2.hpp>

namespace Ares {

	enum class KeyCode : uint16_t;
	enum class MouseCode : uint16_t;

	/**
	 * @class Input
	 * @brief Interface class for handling user input (keyboard and mouse).
	 * 
	 * @details The Input class provides an interface to handle user input, such as
	 * checking if a key is pressed or obtaining mouse position. It includes static
	 * methods for easy access to these input functions. The actual implementation
	 * is platform-specific and provided by derived classes. Input polling methods
	 * are platform-independent and interact with the system-specific implementation
	 * of `IsKeyPressedImpl`, `IsMouseButtonPressedImpl`, and other input handling methods.
	 */
	class Input
	{
	public:
		/**
		 * @brief Checks if a specific key is currently pressed.
		 * 
		 * @details This is a static method that checks the key state by calling the platform-specific
		 * implementation method `IsKeyPressedImpl`.
		 * 
		 * @param key The key code to check.
		 * @return `true` if the key is pressed, otherwise `false`.
		 */
		inline static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }

		/**
		 * @brief Checks if a specific mouse button is currently pressed.
		 * 
		 * @details This is a static method that checks the mouse button state by calling the platform-specific
		 * implementation method `IsMouseButtonPressedImpl`.
		 * 
		 * @param button The mouse button code to check.
		 * @return `true` if the mouse button is pressed, otherwise `false`.
		 */
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }

		/**
		 * @brief Gets the current mouse position in screen coordinates.
		 * 
		 * @details This is a static method that retrieves the mouse position by calling the platform-specific
		 * implementation method `GetMousePositionImpl`.
		 * 
		 * @return The current mouse position in screen coordinates as a `glm::ivec2` (x, y).
		 */
		inline static glm::ivec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

		/**
		 * @brief Gets the current mouse position's X-coordinate in screen coordinates.
		 * 
		 * @details This is a static method that retrieves the X-coordinate of the mouse position
		 * by calling the platform-specific implementation method `GetMouseXImpl`.
		 * 
		 * @return The current mouse X-coordinate in screen coordinates.
		 */
		inline static int32_t GetMouseX() { return s_Instance->GetMouseXImpl(); }

		/**
		 * @brief Gets the current mouse position's Y-coordinate in screen coordinates.
		 *
		 * @details This is a static method that retrieves the Y-coordinate of the mouse position
		 * by calling the platform-specific implementation method `GetMouseYImpl`.
		 *
		 * @return The current mouse Y-coordinate in screen coordinates.
		 */
		inline static int32_t GetMouseY() { return s_Instance->GetMouseYImpl(); }

		/**
		 * @brief Gets the current mouse position in client coordinates (relative to the application window).
		 * 
		 * @details This is a static method that retrieves the mouse position in client coordinates
		 * by calling the platform-specific implementation method `GetMouseClientPositionImpl`.
		 * 
		 * @return The current mouse position in client coordinates as a `glm::ivec2` (x, y).
		 */
		inline static glm::ivec2 GetMouseClientPosition() { return s_Instance->GetMouseClientPositionImpl(); }

		/**
		 * @brief Gets the current mouse position's X-coordinate in client coordinates.
		 * 
		 * @details This is a static method that retrieves the X-coordinate of the mouse position
		 * in client coordinates by calling the platform-specific implementation method `GetMouseClientXImpl`.
		 * 
		 * @return The current mouse X-coordinate in client coordinates.
		 */
		inline static int32_t GetMouseClientX() { return s_Instance->GetMouseClientXImpl(); }

		/**
		 * @brief Gets the current mouse position's Y-coordinate in client coordinates.
		 *
		 * @details This is a static method that retrieves the Y-coordinate of the mouse position
		 * in client coordinates by calling the platform-specific implementation method `GetMouseClientYImpl`.
		 *
		 * @return The current mouse Y-coordinate in client coordinates.
		 */
		inline static int32_t GetMouseClientY() { return s_Instance->GetMouseClientYImpl(); }

		/**
		 * @brief Creates an instance of Input class.
		 * 
		 * @details This static method creates and returns a new instance of the Input class, which
		 * is platform-specific. It ensures that the platform-specific input functionality
		 * is properly initialized.
		 * 
		 * @return A Scope<Input> representing the created instance.
		 */
		static Scope<Input> Create();

		/**
		 * @brief Shuts down the Input system.
		 * 
		 * @details This method releases the resources used by the Input system and resets the static
		 * instance, cleaning up after the input system is no longer needed.
		 */
		inline static void Shutdown() { s_Instance.reset(); }

	protected:
		/**
		 * @brief Platform-specific method to check if a specific key is pressed.
		 * 
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 * 
		 * @param key The key code to check.
		 * @return `true` if the key is pressed, otherwise `false`.
		 */
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;

		/**
		 * @brief Platform specific method to check if a specific mouse button is pressed.
		 * 
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 * 
		 * @param button The mouse button code to be checked.
		 * @return `true` if the mouse button is pressed, otherwise `false`.
		 */
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;

		/**
		 * @brief Platform specific method to get the mouse position in screen coordinates.
		 * 
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 * 
		 * @return The mouse position in screen coordinates as a `glm::ivec2`.
		 */
		virtual glm::ivec2 GetMousePositionImpl() = 0;

		/**
		 * @brief Platform specific method to get the mouse X-coordinate in screen coordinates.
		 * 
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 * 
		 * @return The mouse X-coordinate in screen coordinates.
		 */
		virtual int32_t GetMouseXImpl() = 0;

		/**
		 * @brief Platform specific method to get the mouse Y-coordinate in screen coordinates.
		 *
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 *
		 * @return The mouse Y-coordinate in screen coordinates.
		 */
		virtual int32_t GetMouseYImpl() = 0;

		/**
		 * @brief Platform specific method to get the mouse position in client coordinates.
		 *
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 *
		 * @return The mouse position in client coordinates as a `glm::ivec2`.
		 */
		virtual glm::ivec2 GetMouseClientPositionImpl() = 0;

		/**
		 * @brief Platform specific method to get the mouse X-coordinate in client coordinates.
		 *
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 *
		 * @return The mouse X-coordinate in client coordinates.
		 */
		virtual int32_t GetMouseClientXImpl() = 0;

		/**
		 * @brief Platform specific method to get the mouse Y-coordinate in client coordinates.
		 *
		 * @details This virtual method is meant to be implemented by derived classes for platform-specific input handling.
		 *
		 * @return The mouse Y-coordinate in client coordinates.
		 */
		virtual int32_t GetMouseClientYImpl() = 0;

	private:
		/**
		 * @brief The static instance of the Input class.
		 * 
		 * @details This static variable holds the instance of the platform-specific Input class.
		 * It is used to call the input methods in a platform-independent manner.
		 */
		static Scope<Input> s_Instance;
	};
}