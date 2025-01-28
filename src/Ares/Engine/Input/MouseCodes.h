/**
 * @file MouseCodes.h
 * @brief Contains the mouse codes enumeration and the mouse constants for input handling.
 * 
 * @details This file defines the [MouseCode](#Ares::MouseCode) enum class that lists all the
 * mouse codes used by the application. These mouse codes are based on GLMF and are mapped to
 * individual buttons on the mouse. The [MouseCode](#Ares::MouseCode) enum class allows for
 * easy integration with the [input polling system](#Ares::Systems::Input) in the Ares engine.
 * Additionally, mouse constants are provided for more concise access to specific keys.
 * 
 * The [MouseCode](#Ares::MouseCode) enum is used throughout the engine to represent mouse
 * button presses. These mouse codes are also mapped to mouse event processing logic and may
 * be used for user input handling in various systems like game controls, menus, etc.
 * 
 * @see [Input](#Ares::Systems::Input)
 */
#pragma once

namespace Ares {

	/**
	 * @enum MouseCode
	 * @brief Represents mouse codes used for mouse button press detection.
	 * 
	 * @details This enum defines all the mouse codes for the mouse. The
	 * mouse codes are designed to work with the [Input](#Ares::Systems::Input)
	 * system for easy mapping and event detection.
	 */
	enum class MouseCode : uint8_t
	{
		Unknown = 0,				/**< @htmlonly Mouse button is unknown. @endhtmlonly */
		Button1 = 1,				/**< @c @htmlonly Left Mouse Button @endhtmlonly */
		Button2 = 2,				/**< @c @htmlonly Right Mouse Button @endhtmlonly */
		Button3 = 3,				/**< @c @htmlonly Middle Mouse Button @endhtmlonly */
		Button4 = 4,				/**< @c @htmlonly Mouse Button 4 @endhtmlonly */
		Button5 = 5,				/**< @c @htmlonly Mouse Button 5 @endhtmlonly */
		Button6 = 6,				/**< @c @htmlonly Mouse Button 6 @endhtmlonly */
		Button7 = 7,				/**< @c @htmlonly Mouse Button 7 @endhtmlonly */
		Button8 = 8,				/**< @c @htmlonly Mouse Button 8 @endhtmlonly */
		Button9 = 9,				/**< @c @htmlonly Mouse Button 9 @endhtmlonly */
		Button10 = 10,				/**< @c @htmlonly Mouse Button 10 @endhtmlonly */
		ButtonX1 = 11,				/**< @c @htmlonly Mouse X Button 1 (forward) @endhtmlonly */
		ButtonX2 = 12,				/**< @c @htmlonly Mouse X Button 2 (back) @endhtmlonly */

		ButtonLast = Button8,		/**< @c @htmlonly Last Mouse Button @endhtmlonly */
		ButtonLeft = Button1,		/**< @c @htmlonly Left Mouse Button @endhtmlonly */
		ButtonRight = Button2,		/**< @c @htmlonly Right Mouse Button @endhtmlonly */
		ButtonMiddle = Button3		/**< @c @htmlonly Middle Mouse Button @endhtmlonly */
	};

	/**
	 * @typedef Mouse
	 * @brief Allows the use of `Mouse::Button0` rather than `MouseCode::Button0`.
	 */
	using Mouse = MouseCode;

	/**
	 * @anchor mousecode_ostream_operator
	 * @brief Overloads the stream insertion operator for the MouseCode enum.
	 * 
	 * @details This function allows printing of the MouseCode values as integers.
	 * It is useful for debugging and logging mouse codes.
	 * 
	 * @param os The output stream to insert the mouse code into.
	 * @param mouseCode The mouse code to be added to the stream.
	 * @return The output stream with the mouse code inserted.
	 */
	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

}

#define AR_MOUSE_BUTTON_1      ::Ares::Mouse::Button1
#define AR_MOUSE_BUTTON_2      ::Ares::Mouse::Button2
#define AR_MOUSE_BUTTON_3      ::Ares::Mouse::Button3
#define AR_MOUSE_BUTTON_4      ::Ares::Mouse::Button4
#define AR_MOUSE_BUTTON_5      ::Ares::Mouse::Button5
#define AR_MOUSE_BUTTON_6      ::Ares::Mouse::Button6
#define AR_MOUSE_BUTTON_7      ::Ares::Mouse::Button7
#define AR_MOUSE_BUTTON_8      ::Ares::Mouse::Button8
#define AR_MOUSE_BUTTON_LAST   ::Ares::Mouse::ButtonLast
#define AR_MOUSE_BUTTON_LEFT   ::Ares::Mouse::ButtonLeft
#define AR_MOUSE_BUTTON_RIGHT  ::Ares::Mouse::ButtonRight
#define AR_MOUSE_BUTTON_MIDDLE ::Ares::Mouse::ButtonMiddle