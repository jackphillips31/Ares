#include <arespch.h>
#include "Platform/WinAPI/WinKeyCodes.h"

#include "Engine/Events/KeyCodes.h"

namespace Ares {

	static std::unordered_map<uint32_t, KeyCode> WinKeyMap = {
			{VK_BACK, KeyCode::Backspace},
			{VK_TAB, KeyCode::Tab},
			{VK_RETURN, KeyCode::Enter},
			{VK_PAUSE, KeyCode::Pause},
			{VK_CAPITAL, KeyCode::CapsLock},
			{VK_ESCAPE, KeyCode::Escape},
			{VK_SPACE, KeyCode::Space},
			{VK_PRIOR, KeyCode::PageUp},
			{VK_NEXT, KeyCode::PageDown},
			{VK_END, KeyCode::End},
			{VK_HOME, KeyCode::Home},
			{VK_LEFT, KeyCode::Left},
			{VK_UP, KeyCode::Up},
			{VK_RIGHT, KeyCode::Right},
			{VK_DOWN, KeyCode::Down},
			{VK_SNAPSHOT, KeyCode::PrintScreen},
			{VK_INSERT, KeyCode::Insert},
			{VK_DELETE, KeyCode::Delete},
			{0x30, KeyCode::D0},
			{0x32, KeyCode::D2},
			{0x33, KeyCode::D3},
			{0x34, KeyCode::D4},
			{0x35, KeyCode::D5},
			{0x36, KeyCode::D6},
			{0x37, KeyCode::D7},
			{0x38, KeyCode::D8},
			{0x39, KeyCode::D9},
			{0x41, KeyCode::A},
			{0x42, KeyCode::B},
			{0x43, KeyCode::C},
			{0x44, KeyCode::D},
			{0x45, KeyCode::E},
			{0x46, KeyCode::F},
			{0x47, KeyCode::G},
			{0x48, KeyCode::H},
			{0x49, KeyCode::I},
			{0x4A, KeyCode::J},
			{0x4B, KeyCode::K},
			{0x4C, KeyCode::L},
			{0x4D, KeyCode::M},
			{0x4E, KeyCode::N},
			{0x4F, KeyCode::O},
			{0x50, KeyCode::P},
			{0x51, KeyCode::Q},
			{0x52, KeyCode::R},
			{0x53, KeyCode::S},
			{0x54, KeyCode::T},
			{0x55, KeyCode::U},
			{0x56, KeyCode::V},
			{0x57, KeyCode::W},
			{0x58, KeyCode::X},
			{0x59, KeyCode::Y},
			{0x5A, KeyCode::Z},
			{VK_LWIN, KeyCode::LeftSuper},
			{VK_RWIN, KeyCode::RightSuper},
			{VK_APPS, KeyCode::Menu},
			{VK_NUMPAD0, KeyCode::KP0},
			{VK_NUMPAD1, KeyCode::KP1},
			{VK_NUMPAD2, KeyCode::KP2},
			{VK_NUMPAD3, KeyCode::KP3},
			{VK_NUMPAD4, KeyCode::KP4},
			{VK_NUMPAD5, KeyCode::KP5},
			{VK_NUMPAD6, KeyCode::KP6},
			{VK_NUMPAD7, KeyCode::KP7},
			{VK_NUMPAD8, KeyCode::KP8},
			{VK_NUMPAD9, KeyCode::KP9},
			{VK_MULTIPLY, KeyCode::KPMultiply},
			{VK_ADD, KeyCode::KPAdd},
			{VK_SUBTRACT, KeyCode::KPSubtract},
			{VK_DECIMAL, KeyCode::KPDecimal},
			{VK_DIVIDE, KeyCode::KPDivide},
			{VK_F1, KeyCode::F1},
			{VK_F2, KeyCode::F2},
			{VK_F3, KeyCode::F3},
			{VK_F4, KeyCode::F4},
			{VK_F5, KeyCode::F5},
			{VK_F6, KeyCode::F6},
			{VK_F7, KeyCode::F7},
			{VK_F8, KeyCode::F8},
			{VK_F9, KeyCode::F9},
			{VK_F10, KeyCode::F10},
			{VK_F11, KeyCode::F11},
			{VK_F12, KeyCode::F12},
			{VK_F13, KeyCode::F13},
			{VK_F14, KeyCode::F14},
			{VK_F15, KeyCode::F15},
			{VK_F16, KeyCode::F16},
			{VK_F17, KeyCode::F17},
			{VK_F18, KeyCode::F18},
			{VK_F19, KeyCode::F19},
			{VK_F20, KeyCode::F20},
			{VK_F21, KeyCode::F21},
			{VK_F22, KeyCode::F22},
			{VK_F23, KeyCode::F23},
			{VK_F24, KeyCode::F24},
			{VK_LSHIFT, KeyCode::LeftShift},
			{VK_RSHIFT, KeyCode::RightShift},
			{VK_LCONTROL, KeyCode::LeftControl},
			{VK_RCONTROL, KeyCode::RightControl},
			{VK_LMENU, KeyCode::LeftAlt},
			{VK_RMENU, KeyCode::RightAlt},
			{VK_OEM_1, KeyCode::Semicolon},
			{VK_OEM_PLUS, KeyCode::Equal},
			{VK_OEM_COMMA, KeyCode::Comma},
			{VK_OEM_MINUS, KeyCode::Minus},
			{VK_OEM_PERIOD, KeyCode::Period},
			{VK_OEM_2, KeyCode::Slash},
			{VK_OEM_3, KeyCode::GraveAccent},
			{VK_OEM_4, KeyCode::LeftBracket},
			{VK_OEM_5, KeyCode::Backslash},
			{VK_OEM_6, KeyCode::RightBracket},
			{VK_OEM_7, KeyCode::Apostrophe}
	};

	static std::unordered_map<KeyCode, uint32_t> ReversedWinKeyMap = {
	{KeyCode::Backspace, VK_BACK},
	{KeyCode::Tab, VK_TAB},
	{KeyCode::Enter, VK_RETURN},
	{KeyCode::Pause, VK_PAUSE},
	{KeyCode::CapsLock, VK_CAPITAL},
	{KeyCode::Escape, VK_ESCAPE},
	{KeyCode::Space, VK_SPACE},
	{KeyCode::PageUp, VK_PRIOR},
	{KeyCode::PageDown, VK_NEXT},
	{KeyCode::End, VK_END},
	{KeyCode::Home, VK_HOME},
	{KeyCode::Left, VK_LEFT},
	{KeyCode::Up, VK_UP},
	{KeyCode::Right, VK_RIGHT},
	{KeyCode::Down, VK_DOWN},
	{KeyCode::PrintScreen, VK_SNAPSHOT},
	{KeyCode::Insert, VK_INSERT},
	{KeyCode::Delete, VK_DELETE},
	{KeyCode::D0, 0x30},
	{KeyCode::D2, 0x32},
	{KeyCode::D3, 0x33},
	{KeyCode::D4, 0x34},
	{KeyCode::D5, 0x35},
	{KeyCode::D6, 0x36},
	{KeyCode::D7, 0x37},
	{KeyCode::D8, 0x38},
	{KeyCode::D9, 0x39},
	{KeyCode::A, 0x41},
	{KeyCode::B, 0x42},
	{KeyCode::C, 0x43},
	{KeyCode::D, 0x44},
	{KeyCode::E, 0x45},
	{KeyCode::F, 0x46},
	{KeyCode::G, 0x47},
	{KeyCode::H, 0x48},
	{KeyCode::I, 0x49},
	{KeyCode::J, 0x4A},
	{KeyCode::K, 0x4B},
	{KeyCode::L, 0x4C},
	{KeyCode::M, 0x4D},
	{KeyCode::N, 0x4E},
	{KeyCode::O, 0x4F},
	{KeyCode::P, 0x50},
	{KeyCode::Q, 0x51},
	{KeyCode::R, 0x52},
	{KeyCode::S, 0x53},
	{KeyCode::T, 0x54},
	{KeyCode::U, 0x55},
	{KeyCode::V, 0x56},
	{KeyCode::W, 0x57},
	{KeyCode::X, 0x58},
	{KeyCode::Y, 0x59},
	{KeyCode::Z, 0x5A},
	{KeyCode::LeftSuper, VK_LWIN},
	{KeyCode::RightSuper, VK_RWIN},
	{KeyCode::Menu, VK_APPS},
	{KeyCode::KP0, VK_NUMPAD0},
	{KeyCode::KP1, VK_NUMPAD1},
	{KeyCode::KP2, VK_NUMPAD2},
	{KeyCode::KP3, VK_NUMPAD3},
	{KeyCode::KP4, VK_NUMPAD4},
	{KeyCode::KP5, VK_NUMPAD5},
	{KeyCode::KP6, VK_NUMPAD6},
	{KeyCode::KP7, VK_NUMPAD7},
	{KeyCode::KP8, VK_NUMPAD8},
	{KeyCode::KP9, VK_NUMPAD9},
	{KeyCode::KPMultiply, VK_MULTIPLY},
	{KeyCode::KPAdd, VK_ADD},
	{KeyCode::KPSubtract, VK_SUBTRACT},
	{KeyCode::KPDecimal, VK_DECIMAL},
	{KeyCode::KPDivide, VK_DIVIDE},
	{KeyCode::F1, VK_F1},
	{KeyCode::F2, VK_F2},
	{KeyCode::F3, VK_F3},
	{KeyCode::F4, VK_F4},
	{KeyCode::F5, VK_F5},
	{KeyCode::F6, VK_F6},
	{KeyCode::F7, VK_F7},
	{KeyCode::F8, VK_F8},
	{KeyCode::F9, VK_F9},
	{KeyCode::F10, VK_F10},
	{KeyCode::F11, VK_F11},
	{KeyCode::F12, VK_F12},
	{KeyCode::F13, VK_F13},
	{KeyCode::F14, VK_F14},
	{KeyCode::F15, VK_F15},
	{KeyCode::F16, VK_F16},
	{KeyCode::F17, VK_F17},
	{KeyCode::F18, VK_F18},
	{KeyCode::F19, VK_F19},
	{KeyCode::F20, VK_F20},
	{KeyCode::F21, VK_F21},
	{KeyCode::F22, VK_F22},
	{KeyCode::F23, VK_F23},
	{KeyCode::F24, VK_F24},
	{KeyCode::LeftShift, VK_LSHIFT},
	{KeyCode::RightShift, VK_RSHIFT},
	{KeyCode::LeftControl, VK_LCONTROL},
	{KeyCode::RightControl, VK_RCONTROL},
	{KeyCode::LeftAlt, VK_LMENU},
	{KeyCode::RightAlt, VK_RMENU},
	{KeyCode::Semicolon, VK_OEM_1},
	{KeyCode::Equal, VK_OEM_PLUS},
	{KeyCode::Comma, VK_OEM_COMMA},
	{KeyCode::Minus, VK_OEM_MINUS},
	{KeyCode::Period, VK_OEM_PERIOD},
	{KeyCode::Slash, VK_OEM_2},
	{KeyCode::GraveAccent, VK_OEM_3},
	{KeyCode::LeftBracket, VK_OEM_4},
	{KeyCode::Backslash, VK_OEM_5},
	{KeyCode::RightBracket, VK_OEM_6},
	{KeyCode::Apostrophe, VK_OEM_7}
	};

	KeyCode WinAPIKeyToKeyCode(uint32_t winApiKey)
	{
		auto it = WinKeyMap.find(winApiKey);
		if (it != WinKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return KeyCode::Space;
		}
	}

	uint32_t KeyCodeToWinAPIKey(KeyCode keyCode)
	{
		auto it = ReversedWinKeyMap.find(keyCode);
		if (it != ReversedWinKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return VK_SPACE;
		}
	}

}