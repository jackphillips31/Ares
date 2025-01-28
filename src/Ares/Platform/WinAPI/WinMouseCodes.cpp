#include <arespch.h>
#include "Platform/WinAPI/WinMouseCodes.h"

#include "Engine/Input/MouseCodes.h"

namespace Ares {

	MouseCode WinAPIMouseToMouseCode(uint32_t winApiMouse)
	{
		switch (winApiMouse)
		{
		case VK_LBUTTON: return MouseCode::Button1;
		case VK_RBUTTON: return MouseCode::Button2;
		case VK_MBUTTON: return MouseCode::Button3;
		case VK_XBUTTON1: return MouseCode::ButtonX1;
		case VK_XBUTTON2: return MouseCode::ButtonX2;
		default:
		{
			AR_CORE_WARN("Mouse Code not found.");
			return MouseCode::Unknown;
		}
		}
	}

	uint32_t MouseCodeToWinAPIMouse(MouseCode mouseCode)
	{
		switch (mouseCode)
		{
		case MouseCode::Button1: return VK_LBUTTON;
		case MouseCode::Button2: return VK_RBUTTON;
		case MouseCode::Button3: return VK_MBUTTON;
		case MouseCode::ButtonX1: return VK_XBUTTON1;
		case MouseCode::ButtonX2: return VK_XBUTTON2;
		default:
		{
			AR_CORE_WARN("Mouse Code not found.");
			return 0;
		}
		}
	}

}