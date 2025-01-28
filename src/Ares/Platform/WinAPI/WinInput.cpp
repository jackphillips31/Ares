#include <arespch.h>
#include "Platform/WinAPI/WinInput.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Window.h"
#include "Engine/Input/MousePosition.h"
#include "Platform/WinAPI/WinMouseCodes.h"
#include "Platform/WinAPI/WinKeyCodes.h"

namespace Ares::Systems {

	WinInput::WinInput(Window* window)
	{
		m_Window = static_cast<HWND>(window->GetNativeWindow());
	}

	WinInput::~WinInput()
	{
		m_Window = nullptr;
	}

	bool WinInput::IsKeyPressed(KeyCode key)
	{
		uint32_t winApiKey = KeyCodeToWinAPIKey(key);
		SHORT state = GetAsyncKeyState(winApiKey);

		return (state & 0x8000) != 0;
	}

	bool WinInput::IsMouseButtonPressed(MouseCode button)
	{
		uint32_t winApiMouse = MouseCodeToWinAPIMouse(button);
		SHORT state = GetAsyncKeyState(winApiMouse);

		return (state & 0x8000) != 0;
	}

	MousePosition WinInput::GetMousePosition()
	{
		POINT p;

		if (GetCursorPos(&p))
		{
			return { static_cast<int32_t>(p.x), static_cast<int32_t>(p.y) };
		}
		else
		{
			AR_CORE_WARN("GetCursorPos failed while trying to get the mouse position!");
			return { 0, 0 };
		}
	}

	MousePosition WinInput::GetMouseClientPosition()
	{
		POINT p;

		if (GetCursorPos(&p))
		{
			if (!m_Window) return { 0, 0 };
			ScreenToClient(m_Window, &p);
			return { static_cast<int32_t>(p.x), static_cast<int32_t>(p.y) };
		}
		else
		{
			AR_CORE_WARN("GetCursorPos failed while trying to get the mouse client position!");
			return { 0, 0 };
		}
	}

}