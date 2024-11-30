#include <arespch.h>

#include "Engine/Core/Application.h"

#include "Platform/WinAPI/WinMouseCodes.h"
#include "Platform/WinAPI/WinKeyCodes.h"
#include "Platform/WinAPI/WinInput.h"

namespace Ares {

	bool WinInput::IsKeyPressedImpl(KeyCode keyCode)
	{
		auto window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
		auto winApiKey = KeyCodeToWinAPIKey(keyCode);
		auto state = GetAsyncKeyState(winApiKey);

		return (state & 0x8000) != 0;
	}

	bool WinInput::IsMouseButtonPressedImpl(MouseCode button)
	{
		auto window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
		auto winApiMouse = MouseCodeToWinAPIMouse(button);
		auto state = GetAsyncKeyState(winApiMouse);

		return (state & 0x8000) != 0;
	}

	std::pair<int32_t, int32_t> WinInput::GetMousePositionImpl()
	{
		POINT p;

		if (GetCursorPos(&p))
		{
			return { static_cast<int32_t>(p.x), static_cast<int32_t>(p.y) };
		}
		else
		{
			AR_CORE_WARN("GetCursorPos failed while trying to get mouse position!");
			return { 0, 0 };
		}
	}

	int32_t WinInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	int32_t WinInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

	std::pair<int32_t, int32_t> WinInput::GetMouseClientPositionImpl()
	{
		HWND window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
		POINT p;

		if (GetCursorPos(&p))
		{
			ScreenToClient(window, &p);
			return { static_cast<int32_t>(p.x), static_cast<int32_t>(p.y) };
		}
		else
		{
			AR_CORE_WARN("GetCursorPos failed while trying to get mouse client position!");
			return { 0, 0 };
		}
	}

	int32_t WinInput::GetMouseClientXImpl()
	{
		auto [x, y] = GetMouseClientPositionImpl();
		return x;
	}

	int32_t WinInput::GetMouseClientYImpl()
	{
		auto [x, y] = GetMouseClientPositionImpl();
		return y;
	}

}