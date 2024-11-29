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
		m_Data.Width = 0;
		m_Data.Height = 0;
		m_Data.ClientWidth = props.Width;
		m_Data.ClientHeight = props.Height;
		m_Data.XPos = props.XPos;
		m_Data.YPos = props.YPos;
		m_Data.Flags = props.Flags;
		m_Data.OriginalDisplay = {};

		m_Data.OriginalDisplay.dmSize = sizeof(DEVMODE);
		if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &m_Data.OriginalDisplay) == 0)
		{
			AR_CORE_ERROR("Failed to get the original display settings!");
		}

		WNDCLASS wc = {};
		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = L"AresWindowClass";

		if (props.Icon)
		{
			wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(*(int32_t*)props.Icon));
		}

		RegisterClass(&wc);

		m_Window = CreateWindowExW(
			0,
			wc.lpszClassName,
			std::wstring(props.Title.begin(), props.Title.end()).c_str(),
			0,
			m_Data.XPos, m_Data.YPos,
			m_Data.Width, m_Data.Height,
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
		int32_t processedMessages = 0;
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

	void WinWindow::SetWindowSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height)
	{
		SetWindowPos(
			m_Window, nullptr,
			x, y,
			width, height,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
		);
	}

	void WinWindow::SetClientSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height)
	{
		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(&rect, GetWindowLong(m_Window, GWL_STYLE), 0);

		SetWindowPos(
			m_Window, nullptr,
			x, y,
			rect.right - rect.left, rect.bottom-rect.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
		);
	}

	void WinWindow::SetWindowSettings(uint16_t flags)
	{
		if (m_Data.Flags != flags)
		{
			if (m_Data.Flags & WindowSettings::FullscreenExclusive && !(flags & WindowSettings::FullscreenExclusive))
			{
				ChangeDisplaySettings(&m_Data.OriginalDisplay, 0);
			}
			m_Data.Flags = flags;
			ApplySettings(flags);
		}
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
	}

	void WinWindow::ConfigureFullscreen(uint16_t flags)
	{
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(devMode);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		if (flags & WindowSettings::FullscreenExclusive)
		{
			devMode.dmPelsWidth = m_Data.ClientWidth;
			devMode.dmPelsHeight = m_Data.ClientHeight;
			ChangeDisplaySettings(&devMode, 0);
		}

		SetWindowLongPtr(m_Window, GWL_STYLE, WS_POPUP);

		SetWindowPos(
			m_Window, HWND_TOP,
			0, 0, devMode.dmPelsWidth, devMode.dmPelsHeight,
			SWP_FRAMECHANGED | SWP_NOACTIVATE
		);
		ShowWindow(m_Window, SW_NORMAL);
	}

	void WinWindow::ConfigureWindowed(uint16_t flags)
	{
		uint32_t tempClientWidth = m_Data.ClientWidth;
		uint32_t tempClientHeight = m_Data.ClientHeight;

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
		else if (flags == WindowSettings::FullscreenWindowed)
		{
			windowStyle &= ~WS_MAXIMIZEBOX;
		}

		SetWindowLongPtr(m_Window, GWL_STYLE, windowStyle);

		RECT rect = { 0, 0, tempClientWidth, tempClientHeight };
		AdjustWindowRect(&rect, windowStyle, 0);

		m_Data.Width = rect.right - rect.left;
		m_Data.Height = rect.bottom - rect.top;

		SetWindowPos(
			m_Window, nullptr,
			0, 0,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_NOMOVE
		);

		ShowWindow(m_Window, flags & WindowSettings::Maximized ? SW_MAXIMIZE : SW_NORMAL);
	}

	LRESULT WinWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}
		switch (uMsg)
		{
		case WM_QUIT: {
			WindowCloseEvent event;
			m_Data.EventCallback(event);
			return wParam;
		}
		case WM_DESTROY: {
			ChangeDisplaySettings(&m_Data.OriginalDisplay, 0);
			PostQuitMessage(0);
			return 0;
		}
		case WM_SIZING: {
			RECT* rect = reinterpret_cast<RECT*>(lParam);

			m_Data.Width = rect->right - rect->left;
			m_Data.Height = rect->bottom - rect->top;

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);

			uint32_t clientWidth = clientRect.right - clientRect.left;
			uint32_t clientHeight = clientRect.bottom - clientRect.top;

			if (m_Data.ClientWidth != clientWidth || m_Data.ClientHeight != clientHeight)
			{
				m_Data.ClientWidth = clientWidth;
				m_Data.ClientHeight = clientHeight;

				WindowResizeEvent event(clientWidth, clientHeight);
				if (m_Data.EventCallback)
					m_Data.EventCallback(event);
			}

			return 0;
		}
		case WM_SIZE: {
			uint32_t width = LOWORD(lParam);
			uint32_t height = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				WindowResizeEvent event(0, 0);
				if (m_Data.EventCallback)
					m_Data.EventCallback(event);
			}
			else if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED)
			{
				RECT windowRect;
				GetWindowRect(hwnd, &windowRect);

				uint32_t winWidth = windowRect.right - windowRect.left;
				uint32_t winHeight = windowRect.bottom - windowRect.top;

				m_Data.Width = winWidth;
				m_Data.Height = winHeight;

				m_Data.ClientWidth = width;
				m_Data.ClientHeight = height;

				WindowResizeEvent event(width, height);
				if (m_Data.EventCallback)
					m_Data.EventCallback(event);
			}
			return 0;
		}
		case WM_MOVE: {
			int32_t xpos = LOWORD(lParam);
			int32_t ypos = HIWORD(lParam);

			m_Data.XPos = xpos;
			m_Data.YPos = ypos;

			WindowMovedEvent event(xpos, ypos);
			if (m_Data.EventCallback)
				m_Data.EventCallback(event);

			return 0;
		}
		case WM_ACTIVATE: {
			if (wParam == WA_INACTIVE)
			{
				if (m_Data.Flags & WindowSettings::FullscreenExclusive)
				{
					ChangeDisplaySettings(&m_Data.OriginalDisplay, 0);
				}
				if (m_Data.Flags & WindowSettings::FullscreenExclusive || m_Data.Flags & WindowSettings::Fullscreen)
				{
					ShowWindow(hwnd, SW_MINIMIZE);
				}
			}
			else if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			{
				if (m_Data.Flags & WindowSettings::FullscreenExclusive)
				{
					DEVMODE devMode = m_Data.OriginalDisplay;
					devMode.dmPelsWidth = m_Data.ClientWidth;
					devMode.dmPelsHeight = m_Data.ClientHeight;
					ChangeDisplaySettings(&devMode, 0);
				}
			}
			return 0;
		}
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