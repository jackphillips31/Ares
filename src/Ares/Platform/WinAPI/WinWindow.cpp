#include <arespch.h>
#include <imgui_impl_win32.h>
#include <glad/wgl.h>

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Platform/WinAPI/WinKeyCodes.h"
#include "Platform/WinAPI/WinWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Ares {

	WinWindow::WinWindow(const WindowProps& props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.XPos = props.XPos;
		m_Data.YPos = props.YPos;
		m_Data.Flags = props.Flags;

		WNDCLASS wc = {};
		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = L"AresWindowClass";

		RegisterClass(&wc);

		m_Window = CreateWindowExW(
			0,
			wc.lpszClassName,
			std::wstring(props.Title.begin(), props.Title.end()).c_str(),
			WS_OVERLAPPEDWINDOW,
			props.XPos, props.YPos,
			props.Width, props.Height,
			nullptr, nullptr, wc.hInstance, this
		);

		ApplySettings(props.Flags);

		m_GraphicsContext = GraphicsContext::Create(m_Window);
		m_GraphicsContext->Init();

		SetVSync(true);
	}

	void WinWindow::Shutdown()
	{
		// Destroy the window
		if (m_Window)
		{
			DestroyWindow(m_Window);
		}
	}

	void WinWindow::OnUpdate()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				WindowCloseEvent event;
				m_Data.EventCallback(event);
			}
		}

		m_GraphicsContext->SwapBuffers();
	}

	void WinWindow::SetVSync(bool enabled)
	{
		wglSwapIntervalEXT(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WinWindow::SetWindowPosition(int32_t x, int32_t y)
	{
		m_Data.XPos = x;
		m_Data.YPos = y;

		SetWindowPos(m_Window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	void WinWindow::SetWindowSettings(uint16_t flags)
	{
		m_Data.Flags = flags;
		AR_CORE_INFO("Window Dimensions: {} {}", m_Data.Width, m_Data.Height);
		ApplySettings(flags);
		AR_CORE_INFO("Window Dimensions: {} {}", m_Data.Width, m_Data.Height);
	}

	void WinWindow::ApplySettings(uint16_t flags)
	{
		if (flags & WindowSettings::FullscreenExclusive || flags & WindowSettings::Fullscreen)
		{
			ConfigureFullscreen(flags);
		}
		else
		{
			ConfigureWindowed(flags);
		}

		UpdateWindowBounds(flags);
	}

	void WinWindow::ConfigureFullscreen(uint16_t flags)
	{
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(devMode);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		if (flags & WindowSettings::FullscreenExclusive)
		{
			// Exclusive fullscreen
			devMode.dmPelsWidth = m_Data.Width;
			devMode.dmPelsHeight = m_Data.Height;
			ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
			SetWindowLongPtr(m_Window, GWL_STYLE, WS_POPUP);
		}
		else if (flags & WindowSettings::Fullscreen && flags & WindowSettings::Borderless)
		{
			// Borderless fullscreen
			SetWindowLongPtr(m_Window, GWL_STYLE, WS_POPUP);
		}

		SetWindowPos(
			m_Window, HWND_TOP,
			0, 0, devMode.dmPelsWidth, devMode.dmPelsHeight,
			SWP_FRAMECHANGED | SWP_NOACTIVATE
		);

		ShowWindow(m_Window, SW_MAXIMIZE);
	}

	void WinWindow::ConfigureWindowed(uint16_t flags)
	{
		DWORD windowStyle = WS_OVERLAPPEDWINDOW;
		windowStyle &= ~WS_THICKFRAME;

		if (flags & WindowSettings::Borderless)
		{
			windowStyle = WS_POPUP;
		}
		else if (flags & WindowSettings::Resizable)
		{
			windowStyle |= WS_THICKFRAME;
		}

		SetWindowLongPtr(m_Window, GWL_STYLE, windowStyle);

		if (flags & WindowSettings::Maximized)
		{
			ShowWindow(m_Window, SW_MAXIMIZE);
		}
		else
		{
			ShowWindow(m_Window, SW_NORMAL);
		}
	}

	void WinWindow::UpdateWindowBounds(uint16_t flags)
	{
		RECT rect = { m_Data.XPos, m_Data.YPos, m_Data.XPos + m_Data.Width, m_Data.YPos + m_Data.Height };

		if (flags & WindowSettings::Fullscreen || flags & WindowSettings::FullscreenExclusive || flags & WindowSettings::Maximized)
		{
			HMONITOR monitor = MonitorFromWindow(m_Window, MONITOR_DEFAULTTOPRIMARY);
			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(monitor, &mi);
			rect = mi.rcMonitor;
		}

		AdjustWindowRect(&rect, GetWindowLong(m_Window, GWL_STYLE), 0);
		SetWindowPos(
			m_Window, nullptr,
			rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
		);
	}

	LRESULT WinWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}
		switch (uMsg)
		{
		case WM_DESTROY: {
			PostQuitMessage(0);
			WindowCloseEvent event;
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_SIZING: {
			if (wParam != SIZE_MINIMIZED)
			{
				RECT windowRect;
				GetWindowRect(m_Window, &windowRect);

				m_Data.Width = windowRect.right - windowRect.left;
				m_Data.Height = windowRect.bottom - windowRect.top;

				RECT clientRect;
				GetClientRect(hwnd, &clientRect);

				uint32_t clientWidth = clientRect.right - clientRect.left;
				uint32_t clientHeight = clientRect.bottom - clientRect.top;

				WindowResizeEvent event(clientWidth, clientHeight);
				m_Data.EventCallback(event);
			}
			return 0;
		}
		case WM_SIZE: {
			if (wParam == SIZE_RESTORED)
			{
				if (m_Data.EventCallback)
				{
					WindowResizeEvent event((uint32_t)LOWORD(lParam), (uint32_t)HIWORD(lParam));
					m_Data.EventCallback(event);
				}
			}
			return 0;
		}
		case WM_MOVE: {
			m_Data.XPos = LOWORD(lParam);
			m_Data.YPos = HIWORD(lParam);
			return 0;
		}
		/*
		case WM_ACTIVATE: {
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				ShowWindow(m_Window, SW_MINIMIZE);
				break;
			}
			return 0;
		}
		*/
		case WM_KEYDOWN: {
			KeyCode keyCode = WinAPIKeyToKeyCode(static_cast<uint32_t>(wParam));

			KeyPressedEvent event(keyCode, 0);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_KEYUP: {
			KeyCode keyCode = WinAPIKeyToKeyCode(static_cast<uint32_t>(wParam));

			KeyReleasedEvent event(keyCode);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_CHAR: {
			KeyCode keyCode = WinAPIKeyToKeyCode(static_cast<uint32_t>(wParam));

			KeyTypedEvent event(keyCode);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			MouseButtonPressedEvent event(MouseCode::Button0);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_LBUTTONUP: {
			MouseButtonReleasedEvent event(MouseCode::Button0);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_RBUTTONDOWN: {
			MouseButtonPressedEvent event(MouseCode::Button1);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_RBUTTONUP: {
			MouseButtonReleasedEvent event(MouseCode::Button1);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_MBUTTONDOWN: {
			MouseButtonPressedEvent event(MouseCode::Button2);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_MBUTTONUP: {
			MouseButtonReleasedEvent event(MouseCode::Button2);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_XBUTTONDOWN: {
			MouseButtonPressedEvent event(MouseCode::Button3);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_XBUTTONUP: {
			MouseButtonReleasedEvent event(MouseCode::Button3);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_MOUSEWHEEL: {
			int16_t zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			float yOffset = static_cast<float>(zDelta) / WHEEL_DELTA;

			MouseScrolledEvent event(0.0f, yOffset);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_MOUSEMOVE: {
			int16_t xPos = (int16_t)(short)LOWORD(lParam);
			int16_t yPos = (int16_t)(short)HIWORD(lParam);

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			m_Data.EventCallback(event);
			return 0;
		}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK WinWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WinWindow* window = nullptr;
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			window = static_cast<WinWindow*>(cs->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		}
		else
		{
			window = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		if (window)
		{
			return window->HandleMessage(hwnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}