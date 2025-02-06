/**
 * @file Input.h
 * @brief Interface for handling user input (keyboard and mouse).
 * 
 * @details This header file defines the Input class, which provides platform-independent
 * methods for polling input states, such as checking if keys or mouse buttons are pressed,
 * and getting the position of the mouse. The actual implementation of these methods is platform-specific
 * and is provided by derived classes.
 * 
 * **Example Usage with Application**:
 * ```cpp
 * #include <Ares.h>
 *
 * using namespace Ares;
 *
 * Systems::Input* inputSys = Application::Get().GetSystem<Systems::Input>();
 *
 * if (inputSys->IsKeyPressed(Key::W)) {
 *     // Do something when 'W' key is pressed
 * }
 *
 * MousePosition mousePos = inputSys->GetMousePosition();
 * ```
 *
 * **Example Usage without Application**:
 * ```cpp
 * #include <Engine/Core/Input.h>
 * #include <Engine/Core/Window.h>
 *
 * using namespace Ares;
 *
 * Scope<Window> window = Window::Create();
 * Scope<Systems::Input> inputSys = Systems::Input::Create(window.get());
 *
 * if (inputSys->IsKeyPressed(Key::W)
 * {
 *     // Do something when 'W' key is pressed.
 * }
 * ```
 *
 * @note The Input class is a base class. A platform-specific instance is created using the [Create](#Input::Create) method.
 * <br> **Current Platform Implementations**:
 * @li Windows
 * @warning On **Windows** platforms, [GetMouseClientPosition](#Input::GetMouseClientPosition)
 * requires a valid Window instance. If `nullptr` is passed in the [Create](#Input::Create) method,
 * [GetMouseClientPosition](#Input::GetMouseClientPosition) will return `{ 0, 0 }`.
 */
#pragma once
#include "Engine/Core/Memory.h"
#include "Engine/Core/System.h"

namespace Ares {

	enum class KeyCode : uint16_t;
	enum class MouseCode : uint8_t;
	struct MousePosition;
	class Window;

	namespace Internal {

		class AppAllocator;
		struct Deleter;

	}

	namespace Systems {

		/**
		 * @class Input
		 * @brief Interface class for handling user input (keyboard and mouse).
		 *
		 * @details Provides methods to check key states, mouse button states, and
		 * retrieve mouse positions.<br>
		 * Platform-specific implementations are provided by derived classes through
		 * the [Create](#Input::Create) method.
		 *
		 * **Example Usage with Application**:
		 * ```cpp
		 * #include <Ares.h>
		 *
		 * using namespace Ares;
		 *
		 * Systems::Input* inputSys = Application::Get().GetSystem<Systems::Input>();
		 *
		 * if (inputSys->IsKeyPressed(Key::W)) {
		 *     // Do something when 'W' key is pressed
		 * }
		 *
		 * MousePosition mousePos = inputSys->GetMousePosition();
		 * ```
		 *
		 * **Example Usage without Application**:
		 * ```cpp
		 * #include <Engine/Core/Input.h>
		 * #include <Engine/Core/Window.h>
		 *
		 * using namespace Ares;
		 *
		 * Scope<Window> window = Window::Create();
		 * Scope<Systems::Input> inputSys = Systems::Input::Create(window.get());
		 *
		 * if (inputSys->IsKeyPressed(Key::W)
		 * {
		 *     // Do something when 'W' key is pressed.
		 * }
		 * ```
		 *
		 * @note The Input class is a base class. A platform-specific instance is created using the [Create](#Input::Create) method.
		 * <br> **Current Platform Implementations**:
		 * @li Windows
		 * @warning On **Windows** platforms, [GetMouseClientPosition](#Input::GetMouseClientPosition)
		 * requires a valid Window instance. If `nullptr` is passed in the [Create](#Input::Create) method,
		 * [GetMouseClientPosition](#Input::GetMouseClientPosition) will return `{ 0, 0 }`.
		 * @see Ares::KeyCode, Ares::MouseCode
		 */
		class Input : public Internal::System
		{
		public:
			virtual ~Input() = default;

			/**
			 * @brief Checks if a specific key is currently pressed.
			 *
			 * @details This method check the state of the specified key and returns whether it
			 * is currently pressed.
			 *
			 * @param key The key code to check.
			 * @return `true` if the key is pressed; otherwise, `false`.
			 */
			virtual bool IsKeyPressed(KeyCode key) = 0;

			/**
			 * @brief Checks if a specific mouse button is currently pressed.
			 *
			 * @details This method checks the state of the specified mouse button and returns
			 * whether it is currently pressed.
			 *
			 * @param button The mouse button code to check.
			 * @return `true` if the mouse button is pressed; otherwise, `false`.
			 */
			virtual bool IsMouseButtonPressed(MouseCode button) = 0;

			/**
			 * @brief Gets the current mouse position in screen coordinates.
			 *
			 * @details This method retrieves the current position of the mouse cursor
			 * in screen coordinates.
			 *
			 * @return The current mouse position in screen coordinates as MousePosition.
			 */
			virtual MousePosition GetMousePosition() = 0;

			/**
			 * @brief Gets the current mouse position in client coordinates (relative to the application window).
			 *
			 * @details This method retrieves the current position of the mouse cursor
			 * in client coordinates.
			 *
			 * @return The current mouse position in client coordinates as MousePosition.
			 * @warning On **Windows** platforms, this method requires that a valid Window
			 * instance is provided during the creation of the Input system. Otherwise, it
			 * will not function correctly and will return; `{ 0, 0 }`.
			 */
			virtual MousePosition GetMouseClientPosition() = 0;

			/**
			 * @brief Creates a platform-specific instance of the Input class. Called during Application construction.
			 *
			 * @details This static method creates a platform-specific instance of the Input class.
			 * It ensures that the platform-specific input functionality is properly initialized.
			 *
			 * @param app A pointer to a Window instance.
			 * @return A Scope to the created Input object.
			 * @throws std::runtime_error If platform-specific implementation is unavailable.
			 */
			static AppScope<Input> Create(Window* window = nullptr);

		private:
			template <typename ObjectType, typename... Args>
			friend AppScope<ObjectType> Ares::CreateAppScope(Args&&... args);
		};

	}

}