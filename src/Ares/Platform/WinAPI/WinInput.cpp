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

	std::pair<float, float> WinInput::GetMousePositionImpl()
	{
		AR_CORE_INFO("GetMousePositionImpl");
		auto window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
		POINT p;

		if (GetCursorPos(&p))
		{
			ScreenToClient(window, &p);
			return { (float)p.x, (float)p.y };
		}

		return { 0.0f, 0.0f };
	}

	float WinInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WinInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}