#pragma once

namespace Ares {

	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
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
#define AR_KEY_LEFT_BRACKET    ::Ares::Key::LeftBracket   /* [ */
#define AR_KEY_BACKSLASH       ::Ares::Key::Backslash     /* \ */
#define AR_KEY_RIGHT_BRACKET   ::Ares::Key::RightBracket  /* ] */
#define AR_KEY_GRAVE_ACCENT    ::Ares::Key::GraveAccent   /* ` */
#define AR_KEY_WORLD_1         ::Ares::Key::World1        /* non-US #1 */
#define AR_KEY_WORLD_2         ::Ares::Key::World2        /* non-US #2 */

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