/**
 * @file KeyCodes.h
 * @brief Contains the key codes enumeration and key constants for input handling.
 * 
 * @details This file defines the KeyCode enum class that lists all the key codes used
 * by the application. These key codes are based on GLFW and are mapped to individual
 * keys on the keyboard. The KeyCode enum class allows for easy integration with input
 * handling systems in the Ares engine. Additionally, key constants are provided for more
 * concise access to specific keys.
 * 
 * The KeyCode enum is used throughout the engine to represent key presses.
 * These key codes are also mapped to key event processing logic and may be used
 * for user input handling in various systems like game controls, menus, etc.
 */
#pragma once

namespace Ares {

	/**
	 * @enum KeyCode
	 * @brief Represents key codes used for key press detection.
	 * 
	 * @details This enum defines all the key codes for the keyboard, including
	 * letters, numbers, function keys, control keys, and keypad keys. The
	 * key codes are designed to work with the Ares input system for easy
	 * mapping and event detection.
	 */
	enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39,	/**< ``'`` key */
		Comma = 44,			/**< `,` key */
		Minus = 45,			/**< `-` key */
		Period = 46,		/**< `.` key */
		Slash = 47,			/**< `/` key */

		D0 = 48,			/**< `0` key */
		D1 = 49,			/**< `1` key */
		D2 = 50,			/**< `2` key */
		D3 = 51,			/**< `3` key */
		D4 = 52,			/**< `4` key */
		D5 = 53,			/**< `5` key */
		D6 = 54,			/**< `6` key */
		D7 = 55,			/**< `7` key */
		D8 = 56,			/**< `8` key */
		D9 = 57,			/**< `9` key */

		Semicolon = 59,		/**< `;` key */
		Equal = 61,			/**< `=` key */

		A = 65,				/**< `A` key */
		B = 66,				/**< `B` key */
		C = 67,				/**< `C` key */
		D = 68,				/**< `D` key */
		E = 69,				/**< `E` key */
		F = 70,				/**< `F` key */
		G = 71,				/**< `G` key */
		H = 72,				/**< `H` key */
		I = 73,				/**< `I` key */
		J = 74,				/**< `J` key */
		K = 75,				/**< `K` key */
		L = 76,				/**< `L` key */
		M = 77,				/**< `M` key */
		N = 78,				/**< `N` key */
		O = 79,				/**< `O` key */
		P = 80,				/**< `P` key */
		Q = 81,				/**< `Q` key */
		R = 82,				/**< `R` key */
		S = 83,				/**< `S` key */
		T = 84,				/**< `T` key */
		U = 85,				/**< `U` key */
		V = 86,				/**< `V` key */
		W = 87,				/**< `W` key */
		X = 88,				/**< `X` key */
		Y = 89,				/**< `Y` key */
		Z = 90,				/**< `Z` key */

		LeftBracket = 91,	/**< `[` key */
		Backslash = 92,		/**< `\` key */
		RightBracket = 93,	/**< `]` key */
		GraveAccent = 96,	/**< `` ` `` key */

		World1 = 161,		/**< `non-US #1` */
		World2 = 162,		/**< `non-US #2` */

		/* Function keys */
		Escape = 256,		/**< `Escape` */
		Enter = 257,		/**< `Enter` */
		Tab = 258,			/**< `Tab` */
		Backspace = 259,	/**< `Backspace` */
		Insert = 260,		/**< `Insert` */
		Delete = 261,		/**< `Delete` */
		Right = 262,		/**< `Right` */
		Left = 263,			/**< `Left` */
		Down = 264,			/**< `Down` */
		Up = 265,			/**< `Up` */
		PageUp = 266,		/**< `Page Up` */
		PageDown = 267,		/**< `Page Down` */
		Home = 268,			/**< `Home` */
		End = 269,			/**< `End` */
		CapsLock = 280,		/**< `Caps Lock` */
		ScrollLock = 281,	/**< `Scroll Lock` */
		NumLock = 282,		/**< `Num Lock` */
		PrintScreen = 283,	/**< `Print Screen` */
		Pause = 284,		/**< `Pause` */
		F1 = 290,			/**< `F1` */
		F2 = 291,			/**< `F2` */
		F3 = 292,			/**< `F3` */
		F4 = 293,			/**< `F4` */
		F5 = 294,			/**< `F5` */
		F6 = 295,			/**< `F6` */
		F7 = 296,			/**< `F7` */
		F8 = 297,			/**< `F8` */
		F9 = 298,			/**< `F9` */
		F10 = 299,			/**< `F10` */
		F11 = 300,			/**< `F11` */
		F12 = 301,			/**< `F12` */
		F13 = 302,			/**< `F13` */
		F14 = 303,			/**< `F14` */
		F15 = 304,			/**< `F15` */
		F16 = 305,			/**< `F16` */
		F17 = 306,			/**< `F17` */
		F18 = 307,			/**< `F18` */
		F19 = 308,			/**< `F19` */
		F20 = 309,			/**< `F20` */
		F21 = 310,			/**< `F21` */
		F22 = 311,			/**< `F22` */
		F23 = 312,			/**< `F23` */
		F24 = 313,			/**< `F24` */
		F25 = 314,			/**< `F25` */

		/* Keypad */
		KP0 = 320,			/**< `Keypad: 0` */
		KP1 = 321,			/**< `Keypad: 1` */
		KP2 = 322,			/**< `Keypad: 2` */
		KP3 = 323,			/**< `Keypad: 3` */
		KP4 = 324,			/**< `Keypad: 4` */
		KP5 = 325,			/**< `Keypad: 5` */
		KP6 = 326,			/**< `Keypad: 6` */
		KP7 = 327,			/**< `Keypad: 7` */
		KP8 = 328,			/**< `Keypad: 8` */
		KP9 = 329,			/**< `Keypad: 9` */
		KPDecimal = 330,	/**< `Keypad: .` */
		KPDivide = 331,		/**< `Keypad: /` */
		KPMultiply = 332,	/**< `Keypad: *` */
		KPSubtract = 333,	/**< `Keypad: -` */
		KPAdd = 334,		/**< `Keypad: +` */
		KPEnter = 335,		/**< `Keypad: Enter` */
		KPEqual = 336,		/**< `Keypad: =` */

		LeftShift = 340,	/**< `Left Shift` */
		LeftControl = 341,	/**< `Left Control` */
		LeftAlt = 342,		/**< `Left Alt` */
		LeftSuper = 343,	/**< `Left Super` */
		RightShift = 344,	/**< `Right Shift` */
		RightControl = 345,	/**< `Right Control` */
		RightAlt = 346,		/**< `Right Alt` */
		RightSuper = 347,	/**< `Right Super` */
		Menu = 348			/**< `Menu` */
	};

	/**
	 * @typedef Key
	 * @brief Allows the use of `Key::Enter` rather than `KeyCode::Enter`.
	 */
	using Key = KeyCode;
	
	/**
	 * @anchor keycode_ostream_operator
	 * @brief Overloads the stream insertion operator for KeyCode enum.
	 * 
	 * @details This function allows printing of the KeyCode values as integers.
	 * It is useful for debugging and logging key codes.
	 * 
	 * @param os The output stream to insert the key code into.
	 * @param keyCode The key code to be printed.
	 * @return The output stream with the key code inserted.
	 */
	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		return os << static_cast<int32_t>(keyCode);
	}

}

#define AR_KEY_SPACE           ::Ares::Key::Space
#define AR_KEY_APOSTROPHE      ::Ares::Key::Apostrophe    /* ' */
#define AR_KEY_COMMA           ::Ares::Key::Comma         /* , */
#define AR_KEY_MINUS           ::Ares::Key::Minus         /* - */
#define AR_KEY_PERIOD          ::Ares::Key::Period        /* . */
#define AR_KEY_SLASH           ::Ares::Key::Slash         /* / */
#define AR_KEY_0               ::Ares::Key::D0
#define AR_KEY_1               ::Ares::Key::D1
#define AR_KEY_2               ::Ares::Key::D2
#define AR_KEY_3               ::Ares::Key::D3
#define AR_KEY_4               ::Ares::Key::D4
#define AR_KEY_5               ::Ares::Key::D5
#define AR_KEY_6               ::Ares::Key::D6
#define AR_KEY_7               ::Ares::Key::D7
#define AR_KEY_8               ::Ares::Key::D8
#define AR_KEY_9               ::Ares::Key::D9
#define AR_KEY_SEMICOLON       ::Ares::Key::Semicolon     /* ; */
#define AR_KEY_EQUAL           ::Ares::Key::Equal         /* = */
#define AR_KEY_A               ::Ares::Key::A
#define AR_KEY_B               ::Ares::Key::B
#define AR_KEY_C               ::Ares::Key::C
#define AR_KEY_D               ::Ares::Key::D
#define AR_KEY_E               ::Ares::Key::E
#define AR_KEY_F               ::Ares::Key::F
#define AR_KEY_G               ::Ares::Key::G
#define AR_KEY_H               ::Ares::Key::H
#define AR_KEY_I               ::Ares::Key::I
#define AR_KEY_J               ::Ares::Key::J
#define AR_KEY_K               ::Ares::Key::K
#define AR_KEY_L               ::Ares::Key::L
#define AR_KEY_M               ::Ares::Key::M
#define AR_KEY_N               ::Ares::Key::N
#define AR_KEY_O               ::Ares::Key::O
#define AR_KEY_P               ::Ares::Key::P
#define AR_KEY_Q               ::Ares::Key::Q
#define AR_KEY_R               ::Ares::Key::R
#define AR_KEY_S               ::Ares::Key::S
#define AR_KEY_T               ::Ares::Key::T
#define AR_KEY_U               ::Ares::Key::U
#define AR_KEY_V               ::Ares::Key::V
#define AR_KEY_W               ::Ares::Key::W
#define AR_KEY_X               ::Ares::Key::X
#define AR_KEY_Y               ::Ares::Key::Y
#define AR_KEY_Z               ::Ares::Key::Z
#define AR_KEY_LEFT_BRACKET    ::Ares::Key::LeftBracket		/* [ */
#define AR_KEY_BACKSLASH       ::Ares::Key::Backslash		/* \ */
#define AR_KEY_RIGHT_BRACKET   ::Ares::Key::RightBracket	/* ] */
#define AR_KEY_GRAVE_ACCENT    ::Ares::Key::GraveAccent		/* ` */
#define AR_KEY_WORLD_1         ::Ares::Key::World1			/* non-US #1 */
#define AR_KEY_WORLD_2         ::Ares::Key::World2			/* non-US #2 */

#define AR_KEY_ESCAPE          ::Ares::Key::Escape
#define AR_KEY_ENTER           ::Ares::Key::Enter
#define AR_KEY_TAB             ::Ares::Key::Tab
#define AR_KEY_BACKSPACE       ::Ares::Key::Backspace
#define AR_KEY_INSERT          ::Ares::Key::Insert
#define AR_KEY_DELETE          ::Ares::Key::Delete
#define AR_KEY_RIGHT           ::Ares::Key::Right
#define AR_KEY_LEFT            ::Ares::Key::Left
#define AR_KEY_DOWN            ::Ares::Key::Down
#define AR_KEY_UP              ::Ares::Key::Up
#define AR_KEY_PAGE_UP         ::Ares::Key::PageUp
#define AR_KEY_PAGE_DOWN       ::Ares::Key::PageDown
#define AR_KEY_HOME            ::Ares::Key::Home
#define AR_KEY_END             ::Ares::Key::End
#define AR_KEY_CAPS_LOCK       ::Ares::Key::CapsLock
#define AR_KEY_SCROLL_LOCK     ::Ares::Key::ScrollLock
#define AR_KEY_NUM_LOCK        ::Ares::Key::NumLock
#define AR_KEY_PRINT_SCREEN    ::Ares::Key::PrintScreen
#define AR_KEY_PAUSE           ::Ares::Key::Pause
#define AR_KEY_F1              ::Ares::Key::F1
#define AR_KEY_F2              ::Ares::Key::F2
#define AR_KEY_F3              ::Ares::Key::F3
#define AR_KEY_F4              ::Ares::Key::F4
#define AR_KEY_F5              ::Ares::Key::F5
#define AR_KEY_F6              ::Ares::Key::F6
#define AR_KEY_F7              ::Ares::Key::F7
#define AR_KEY_F8              ::Ares::Key::F8
#define AR_KEY_F9              ::Ares::Key::F9
#define AR_KEY_F10             ::Ares::Key::F10
#define AR_KEY_F11             ::Ares::Key::F11
#define AR_KEY_F12             ::Ares::Key::F12
#define AR_KEY_F13             ::Ares::Key::F13
#define AR_KEY_F14             ::Ares::Key::F14
#define AR_KEY_F15             ::Ares::Key::F15
#define AR_KEY_F16             ::Ares::Key::F16
#define AR_KEY_F17             ::Ares::Key::F17
#define AR_KEY_F18             ::Ares::Key::F18
#define AR_KEY_F19             ::Ares::Key::F19
#define AR_KEY_F20             ::Ares::Key::F20
#define AR_KEY_F21             ::Ares::Key::F21
#define AR_KEY_F22             ::Ares::Key::F22
#define AR_KEY_F23             ::Ares::Key::F23
#define AR_KEY_F24             ::Ares::Key::F24
#define AR_KEY_F25             ::Ares::Key::F25

/* Keypad */
#define AR_KEY_KP_0            ::Ares::Key::KP0
#define AR_KEY_KP_1            ::Ares::Key::KP1
#define AR_KEY_KP_2            ::Ares::Key::KP2
#define AR_KEY_KP_3            ::Ares::Key::KP3
#define AR_KEY_KP_4            ::Ares::Key::KP4
#define AR_KEY_KP_5            ::Ares::Key::KP5
#define AR_KEY_KP_6            ::Ares::Key::KP6
#define AR_KEY_KP_7            ::Ares::Key::KP7
#define AR_KEY_KP_8            ::Ares::Key::KP8
#define AR_KEY_KP_9            ::Ares::Key::KP9
#define AR_KEY_KP_DECIMAL      ::Ares::Key::KPDecimal
#define AR_KEY_KP_DIVIDE       ::Ares::Key::KPDivide
#define AR_KEY_KP_MULTIPLY     ::Ares::Key::KPMultiply
#define AR_KEY_KP_SUBTRACT     ::Ares::Key::KPSubtract
#define AR_KEY_KP_ADD          ::Ares::Key::KPAdd
#define AR_KEY_KP_ENTER        ::Ares::Key::KPEnter
#define AR_KEY_KP_EQUAL        ::Ares::Key::KPEqual

#define AR_KEY_LEFT_SHIFT      ::Ares::Key::LeftShift
#define AR_KEY_LEFT_CONTROL    ::Ares::Key::LeftControl
#define AR_KEY_LEFT_ALT        ::Ares::Key::LeftAlt
#define AR_KEY_LEFT_SUPER      ::Ares::Key::LeftSuper
#define AR_KEY_RIGHT_SHIFT     ::Ares::Key::RightShift
#define AR_KEY_RIGHT_CONTROL   ::Ares::Key::RightControl
#define AR_KEY_RIGHT_ALT       ::Ares::Key::RightAlt
#define AR_KEY_RIGHT_SUPER     ::Ares::Key::RightSuper
#define AR_KEY_MENU            ::Ares::Key::Menu