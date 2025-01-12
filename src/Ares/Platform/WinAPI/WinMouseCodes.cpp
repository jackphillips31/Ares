#include <arespch.h>
#include "Platform/WinAPI/WinMouseCodes.h"

#include "Engine/Events/MouseCodes.h"

namespace Ares {

	static std::unordered_map<uint32_t, MouseCode> WinMouseMap = {
		{WM_LBUTTONDOWN, MouseCode::Button0},
		{WM_LBUTTONUP, MouseCode::Button0},
		{WM_RBUTTONDOWN, MouseCode::Button1},
		{WM_RBUTTONUP, MouseCode::Button1},
		{WM_MBUTTONDOWN, MouseCode::Button2},
		{WM_MBUTTONUP, MouseCode::Button2},
		{WM_XBUTTONDOWN, MouseCode::Button3},
		{WM_XBUTTONUP, MouseCode::Button3}
	};

	static std::unordered_map<MouseCode, uint32_t> ReversedWinMouseMap = {
		{MouseCode::Button0, WM_LBUTTONDOWN},
		{MouseCode::Button1, WM_RBUTTONDOWN},
		{MouseCode::Button2, WM_MBUTTONDOWN},
		{MouseCode::Button3, WM_XBUTTONDOWN}
	};

	MouseCode WinAPIMouseToMouseCode(uint32_t winApiMouse)
	{
		auto it = WinMouseMap.find(winApiMouse);
		if (it != WinMouseMap.end())
		{
			return it->second;
		}
		else
		{
			return MouseCode::Button7;
		}
	}

	uint32_t MouseCodeToWinAPIMouse(MouseCode mouseCode)
	{
		auto it = ReversedWinMouseMap.find(mouseCode);
		if (it != ReversedWinMouseMap.end())
		{
			return it->second;
		}
		else
		{
			return 0;
		}
	}

}