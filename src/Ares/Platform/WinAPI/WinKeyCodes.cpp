#include <arespch.h>
#include "Platform/WinAPI/WinKeyCodes.h"

#include "Engine/Input/KeyCodes.h"

namespace Ares {

	KeyCode WinAPIKeyToKeyCode(uint32_t winApiKey)
	{
		switch (winApiKey)
		{
		case VK_BACK: return KeyCode::Backspace;
		case VK_TAB: return KeyCode::Tab;
		case VK_RETURN: return KeyCode::Enter;
		case VK_PAUSE: return KeyCode::Pause;
		case VK_CAPITAL: return KeyCode::CapsLock;
		case VK_ESCAPE: return KeyCode::Escape;
		case VK_SPACE: return KeyCode::Space;
		case VK_PRIOR: return KeyCode::PageUp;
		case VK_NEXT: return KeyCode::PageDown;
		case VK_END: return KeyCode::End;
		case VK_HOME: return KeyCode::Home;
		case VK_LEFT: return KeyCode::Left;
		case VK_UP: return KeyCode::Up;
		case VK_RIGHT: return KeyCode::Right;
		case VK_DOWN: return KeyCode::Down;
		case VK_SNAPSHOT: return KeyCode::PrintScreen;
		case VK_INSERT: return KeyCode::Insert;
		case VK_DELETE: return KeyCode::Delete;
		case 0x30: return KeyCode::D0;
		case 0x32: return KeyCode::D2;
		case 0x33: return KeyCode::D3;
		case 0x34: return KeyCode::D4;
		case 0x35: return KeyCode::D5;
		case 0x36: return KeyCode::D6;
		case 0x37: return KeyCode::D7;
		case 0x38: return KeyCode::D8;
		case 0x39: return KeyCode::D9;
		case 0x41: return KeyCode::A;
		case 0x42: return KeyCode::B;
		case 0x43: return KeyCode::C;
		case 0x44: return KeyCode::D;
		case 0x45: return KeyCode::E;
		case 0x46: return KeyCode::F;
		case 0x47: return KeyCode::G;
		case 0x48: return KeyCode::H;
		case 0x49: return KeyCode::I;
		case 0x4A: return KeyCode::J;
		case 0x4B: return KeyCode::K;
		case 0x4C: return KeyCode::L;
		case 0x4D: return KeyCode::M;
		case 0x4E: return KeyCode::N;
		case 0x4F: return KeyCode::O;
		case 0x50: return KeyCode::P;
		case 0x51: return KeyCode::Q;
		case 0x52: return KeyCode::R;
		case 0x53: return KeyCode::S;
		case 0x54: return KeyCode::T;
		case 0x55: return KeyCode::U;
		case 0x56: return KeyCode::V;
		case 0x57: return KeyCode::W;
		case 0x58: return KeyCode::X;
		case 0x59: return KeyCode::Y;
		case 0x5A: return KeyCode::Z;
		case VK_LWIN: return KeyCode::LeftSuper;
		case VK_RWIN: return KeyCode::RightSuper;
		case VK_APPS: return KeyCode::Menu;
		case VK_NUMPAD0: return KeyCode::KP0;
		case VK_NUMPAD1: return KeyCode::KP1;
		case VK_NUMPAD2: return KeyCode::KP2;
		case VK_NUMPAD3: return KeyCode::KP3;
		case VK_NUMPAD4: return KeyCode::KP4;
		case VK_NUMPAD5: return KeyCode::KP5;
		case VK_NUMPAD6: return KeyCode::KP6;
		case VK_NUMPAD7: return KeyCode::KP7;
		case VK_NUMPAD8: return KeyCode::KP8;
		case VK_NUMPAD9: return KeyCode::KP9;
		case VK_MULTIPLY: return KeyCode::KPMultiply;
		case VK_ADD: return KeyCode::KPAdd;
		case VK_SUBTRACT: return KeyCode::KPSubtract;
		case VK_DECIMAL: return KeyCode::KPDecimal;
		case VK_DIVIDE: return KeyCode::KPDivide;
		case VK_F1: return KeyCode::F1;
		case VK_F2: return KeyCode::F2;
		case VK_F3: return KeyCode::F3;
		case VK_F4: return KeyCode::F4;
		case VK_F5: return KeyCode::F5;
		case VK_F6: return KeyCode::F6;
		case VK_F7: return KeyCode::F7;
		case VK_F8: return KeyCode::F8;
		case VK_F9: return KeyCode::F9;
		case VK_F10: return KeyCode::F10;
		case VK_F11: return KeyCode::F11;
		case VK_F12: return KeyCode::F12;
		case VK_F13: return KeyCode::F13;
		case VK_F14: return KeyCode::F14;
		case VK_F15: return KeyCode::F15;
		case VK_F16: return KeyCode::F16;
		case VK_F17: return KeyCode::F17;
		case VK_F18: return KeyCode::F18;
		case VK_F19: return KeyCode::F19;
		case VK_F20: return KeyCode::F20;
		case VK_F21: return KeyCode::F21;
		case VK_F22: return KeyCode::F22;
		case VK_F23: return KeyCode::F23;
		case VK_F24: return KeyCode::F24;
		case VK_LSHIFT: return KeyCode::LeftShift;
		case VK_RSHIFT: return KeyCode::RightShift;
		case VK_LCONTROL: return KeyCode::LeftControl;
		case VK_RCONTROL: return KeyCode::RightControl;
		case VK_LMENU: return KeyCode::LeftAlt;
		case VK_RMENU: return KeyCode::RightAlt;
		case VK_OEM_1: return KeyCode::Semicolon;
		case VK_OEM_PLUS: return KeyCode::Equal;
		case VK_OEM_COMMA: return KeyCode::Comma;
		case VK_OEM_MINUS: return KeyCode::Minus;
		case VK_OEM_PERIOD: return KeyCode::Period;
		case VK_OEM_2: return KeyCode::Slash;
		case VK_OEM_3: return KeyCode::GraveAccent;
		case VK_OEM_4: return KeyCode::LeftBracket;
		case VK_OEM_5: return KeyCode::Backslash;
		case VK_OEM_6: return KeyCode::RightBracket;
		case VK_OEM_7: return KeyCode::Apostrophe;
		default:
		{
			AR_CORE_WARN("Key Code not found");
			return KeyCode::Unknown;
		}
		}
	}

	uint32_t KeyCodeToWinAPIKey(KeyCode keyCode)
	{
		switch (keyCode)
		{
			case KeyCode::Backspace: return VK_BACK;
			case KeyCode::Tab: return VK_TAB;
			case KeyCode::Enter: return VK_RETURN;
			case KeyCode::Pause: return VK_PAUSE;
			case KeyCode::CapsLock: return VK_CAPITAL;
			case KeyCode::Escape: return VK_ESCAPE;
			case KeyCode::Space: return VK_SPACE;
			case KeyCode::PageUp: return VK_PRIOR;
			case KeyCode::PageDown: return VK_NEXT;
			case KeyCode::End: return VK_END;
			case KeyCode::Home: return VK_HOME;
			case KeyCode::Left: return VK_LEFT;
			case KeyCode::Up: return VK_UP;
			case KeyCode::Right: return VK_RIGHT;
			case KeyCode::Down: return VK_DOWN;
			case KeyCode::PrintScreen: return VK_SNAPSHOT;
			case KeyCode::Insert: return VK_INSERT;
			case KeyCode::Delete: return VK_DELETE;
			case KeyCode::D0: return 0x30;
			case KeyCode::D1: return 0x31;
			case KeyCode::D2: return 0x32;
			case KeyCode::D3: return 0x33;
			case KeyCode::D4: return 0x34;
			case KeyCode::D5: return 0x35;
			case KeyCode::D6: return 0x36;
			case KeyCode::D7: return 0x37;
			case KeyCode::D8: return 0x38;
			case KeyCode::D9: return 0x39;
			case KeyCode::A: return 0x41;
			case KeyCode::B: return 0x42;
			case KeyCode::C: return 0x43;
			case KeyCode::D: return 0x44;
			case KeyCode::E: return 0x45;
			case KeyCode::F: return 0x46;
			case KeyCode::G: return 0x47;
			case KeyCode::H: return 0x48;
			case KeyCode::I: return 0x49;
			case KeyCode::J: return 0x4A;
			case KeyCode::K: return 0x4B;
			case KeyCode::L: return 0x4C;
			case KeyCode::M: return 0x4D;
			case KeyCode::N: return 0x4E;
			case KeyCode::O: return 0x4F;
			case KeyCode::P: return 0x50;
			case KeyCode::Q: return 0x51;
			case KeyCode::R: return 0x52;
			case KeyCode::S: return 0x53;
			case KeyCode::T: return 0x54;
			case KeyCode::U: return 0x55;
			case KeyCode::V: return 0x56;
			case KeyCode::W: return 0x57;
			case KeyCode::X: return 0x58;
			case KeyCode::Y: return 0x59;
			case KeyCode::Z: return 0x5A;
			case KeyCode::LeftSuper: return VK_LWIN;
			case KeyCode::RightSuper: return VK_RWIN;
			case KeyCode::Menu: return VK_APPS;
			case KeyCode::KP0: return VK_NUMPAD0;
			case KeyCode::KP1: return VK_NUMPAD1;
			case KeyCode::KP2: return VK_NUMPAD2;
			case KeyCode::KP3: return VK_NUMPAD3;
			case KeyCode::KP4: return VK_NUMPAD4;
			case KeyCode::KP5: return VK_NUMPAD5;
			case KeyCode::KP6: return VK_NUMPAD6;
			case KeyCode::KP7: return VK_NUMPAD7;
			case KeyCode::KP8: return VK_NUMPAD8;
			case KeyCode::KP9: return VK_NUMPAD9;
			case KeyCode::KPMultiply: return VK_MULTIPLY;
			case KeyCode::KPAdd: return VK_ADD;
			case KeyCode::KPSubtract: return VK_SUBTRACT;
			case KeyCode::KPDecimal: return VK_DECIMAL;
			case KeyCode::KPDivide: return VK_DIVIDE;
			case KeyCode::F1: return VK_F1;
			case KeyCode::F2: return VK_F2;
			case KeyCode::F3: return VK_F3;
			case KeyCode::F4: return VK_F4;
			case KeyCode::F5: return VK_F5;
			case KeyCode::F6: return VK_F6;
			case KeyCode::F7: return VK_F7;
			case KeyCode::F8: return VK_F8;
			case KeyCode::F9: return VK_F9;
			case KeyCode::F10: return VK_F10;
			case KeyCode::F11: return VK_F11;
			case KeyCode::F12: return VK_F12;
			case KeyCode::F13: return VK_F13;
			case KeyCode::F14: return VK_F14;
			case KeyCode::F15: return VK_F15;
			case KeyCode::F16: return VK_F16;
			case KeyCode::F17: return VK_F17;
			case KeyCode::F18: return VK_F18;
			case KeyCode::F19: return VK_F19;
			case KeyCode::F20: return VK_F20;
			case KeyCode::F21: return VK_F21;
			case KeyCode::F22: return VK_F22;
			case KeyCode::F23: return VK_F23;
			case KeyCode::F24: return VK_F24;
			case KeyCode::LeftShift: return VK_LSHIFT;
			case KeyCode::RightShift: return VK_RSHIFT;
			case KeyCode::LeftControl: return VK_LCONTROL;
			case KeyCode::RightControl: return VK_RCONTROL;
			case KeyCode::LeftAlt: return VK_LMENU;
			case KeyCode::RightAlt: return VK_RMENU;
			case KeyCode::Semicolon: return VK_OEM_1;
			case KeyCode::Equal: return VK_OEM_PLUS;
			case KeyCode::Comma: return VK_OEM_COMMA;
			case KeyCode::Minus: return VK_OEM_MINUS;
			case KeyCode::Period: return VK_OEM_PERIOD;
			case KeyCode::Slash: return VK_OEM_2;
			case KeyCode::GraveAccent: return VK_OEM_3;
			case KeyCode::LeftBracket: return VK_OEM_4;
			case KeyCode::Backslash: return VK_OEM_5;
			case KeyCode::RightBracket: return VK_OEM_6;
			case KeyCode::Apostrophe: return VK_OEM_7;
			default:
			{
				AR_CORE_WARN("Key Code not found.");
				return 0;
			}
		}
	}

}