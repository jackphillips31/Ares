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

		// Register the window class
		WNDCLASS wc = {};
		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = L"AresWindowClass";

		RegisterClass(&wc);

		int windowStyle = 0;
		int windowExStyle = 0;

		switch (m_Data.Flags)
		{
		case WindowFlags::None: {
			windowStyle = WS_OVERLAPPEDWINDOW;
			break;
		}
		case WindowFlags::Borderless: {
			windowStyle = WS_POPUP;
			windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			break;
		}
		case WindowFlags::Fullscreen: {
			POINT pt = { 0, 0 };
			HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(hMonitor, &mi))
			{
				m_Data.Width = (uint32_t)mi.rcMonitor.right - mi.rcMonitor.left;
				m_Data.Height = (uint32_t)(mi.rcMonitor.bottom - mi.rcMonitor.top);
				m_Data.XPos = mi.rcMonitor.left;
				m_Data.YPos = mi.rcMonitor.top;

				DEVMODE dm = {};
				dm.dmSize = sizeof(dm);
				if (EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &dm))
				{
					dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
					if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
					{
						AR_CORE_ERROR("Failed to switch to exclusive fullscreen!");
					}
				}
			}
			windowStyle = WS_POPUP | WS_VISIBLE;
			break;
		}
		case WindowFlags::BorderlessFullscreen: {
			POINT pt = { 0, 0 };
			HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(hMonitor, &mi))
			{
				m_Data.Width = mi.rcMonitor.right - mi.rcMonitor.left;
				m_Data.Height = mi.rcMonitor.bottom - mi.rcMonitor.top;
				m_Data.XPos = mi.rcMonitor.left;
				m_Data.YPos = mi.rcMonitor.top;
			}
			windowStyle = WS_POPUP;
			windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			break;
		}
		default: {
			windowStyle = WS_OVERLAPPEDWINDOW;
			break;
		}
		}

		// Create the window
		m_Window = CreateWindowEx(
			windowExStyle,
			wc.lpszClassName,
			std::wstring(props.Title.begin(), props.Title.end()).c_str(),
			windowStyle,
			m_Data.XPos, m_Data.YPos,
			m_Data.Width, m_Data.Height,
			nullptr, nullptr, wc.hInstance, this
		);

		if (m_Window == nullptr)
		{
			AR_CORE_ASSERT(false, "Failed to create window!");
		}

		// Show the window
		ShowWindow(m_Window, SW_SHOW);
		UpdateWindow(m_Window);

		// Initialize OpenGL context
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

	std::pair<int, int> WinWindow::GetCursorPos() const
	{
		POINT cursorPos;
		::GetCursorPos(&cursorPos);

		return std::pair<int, int>(cursorPos.x, cursorPos.y);
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

	void WinWindow::SetWindowPosition(int x, int y)
	{
		m_Data.XPos = x;
		m_Data.YPos = y;

		SetWindowPos(m_Window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	void WinWindow::SetWindowSettings(WindowFlags flags)
	{
		switch (flags)
		{
		case WindowFlags::None: {
			if (hasFlag(m_Data.Flags, WindowFlags::Borderless) || hasFlag(m_Data.Flags, WindowFlags::Fullscreen))
			{
				if (hasFlag(m_Data.Flags, WindowFlags::Fullscreen))
					ExitFullscreenExclusive();
				SetWindowed();
				m_Data.Flags & ~WindowFlags::Borderless;
				m_Data.Flags & ~WindowFlags::Fullscreen;
			}
			break;
		}
		case WindowFlags::Borderless: {
			if (!hasFlag(m_Data.Flags, WindowFlags::Borderless))
			{
				if (hasFlag(m_Data.Flags, WindowFlags::Fullscreen))
					ExitFullscreenExclusive();
				SetBorderless();
				m_Data.Flags |= WindowFlags::Borderless;
			}
			break;
		}
		case WindowFlags::Fullscreen: {
			if (!hasFlag(m_Data.Flags, WindowFlags::Fullscreen))
			{
				SetFullscreenExclusive();
				m_Data.Flags |= WindowFlags::Fullscreen;
			}
			break;
		}
		case WindowFlags::BorderlessFullscreen: {
			if (!hasFlag(m_Data.Flags, WindowFlags::BorderlessFullscreen))
			{
				if (hasFlag(m_Data.Flags, WindowFlags::Fullscreen))
					ExitFullscreenExclusive();
				SetBorderlessFullscreen();
				m_Data.Flags |= WindowFlags::BorderlessFullscreen;
			}
			break;
		}
		default: {
			AR_CORE_WARN("Window setting not found! Will use default");
			if (hasFlag(m_Data.Flags, WindowFlags::Borderless) || hasFlag(m_Data.Flags, WindowFlags::Fullscreen))
			{
				SetWindowed();
				m_Data.Flags & ~WindowFlags::Borderless;
				m_Data.Flags & ~WindowFlags::Fullscreen;
			}
			break;
		}
		}
	}

	void WinWindow::SetWindowMode(DWORD newStyle, DWORD newExStyle, int width, int height, int x, int y)
	{
		AR_CORE_ERROR("SET WINDOW MODE");
		SetWindowLong(m_Window, GWL_STYLE, newStyle);
		SetWindowLong(m_Window, GWL_EXSTYLE, newExStyle);

		SetWindowPos(m_Window, 0, x, y, width, height, SWP_NOZORDER);
		ShowWindow(m_Window, SW_SHOW);
	}

	void WinWindow::SetWindowed()
	{
		SetWindowMode(WS_OVERLAPPEDWINDOW, 0, m_Data.Width, m_Data.Height, m_Data.XPos, m_Data.YPos);
	}

	void WinWindow::SetBorderless()
	{
		SetWindowMode(WS_POPUP, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, m_Data.Width, m_Data.Height, m_Data.XPos, m_Data.YPos);
	}

	void WinWindow::SetBorderlessFullscreen()
	{
		HMONITOR hMonitor = MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (GetMonitorInfo(hMonitor, &mi))
		{
			int width = mi.rcMonitor.right - mi.rcMonitor.left;
			int height = mi.rcMonitor.bottom - mi.rcMonitor.top;
			SetWindowMode(WS_POPUP, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, width, height, mi.rcMonitor.left, mi.rcMonitor.top);
		}
	}

	void WinWindow::SetFullscreenExclusive(int refreshRate)
	{
		HMONITOR hMonitor = MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (GetMonitorInfo(hMonitor, &mi))
		{
			int width = mi.rcMonitor.right - mi.rcMonitor.left;
			int height = mi.rcMonitor.bottom - mi.rcMonitor.top;

			DEVMODE dm = {};
			dm.dmSize = sizeof(dm);
			dm.dmPelsWidth = width;
			dm.dmPelsHeight = height;
			dm.dmBitsPerPel = 32;
			dm.dmDisplayFrequency = refreshRate;
			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

			if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
			{
				SetWindowMode(WS_POPUP | WS_VISIBLE, 0, width, height);
			}
			else
			{
				AR_CORE_ERROR("Failed to switch to exclusive fullscreen!");
			}
		}
	}

	void WinWindow::ExitFullscreenExclusive()
	{
		ChangeDisplaySettings(nullptr, 0);
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

				WindowResizeEvent event((unsigned int)LOWORD(lParam), (unsigned int)HIWORD(lParam));
				m_Data.EventCallback(event);
			}
			return 0;
		}
		case WM_SIZE: {
			if (wParam == SIZE_RESTORED)
			{
				if (m_Data.EventCallback)
				{
					WindowResizeEvent event((unsigned int)LOWORD(lParam), (unsigned int)HIWORD(lParam));
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
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			float yOffset = static_cast<float>(zDelta) / WHEEL_DELTA;

			MouseScrolledEvent event(0.0f, yOffset);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_MOUSEHWHEEL: {
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			float xOffset = static_cast<float>(zDelta) / WHEEL_DELTA;

			MouseScrolledEvent event(xOffset, 0.0f);
			m_Data.EventCallback(event);
			return 0;
		}
		case WM_MOUSEMOVE: {
			int xPos = (int)(short)LOWORD(lParam);
			int yPos = (int)(short)HIWORD(lParam);

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