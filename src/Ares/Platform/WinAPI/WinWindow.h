#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Renderer/GraphicsContext.h"

namespace Ares {

	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProps& props);
		virtual ~WinWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline std::pair<int, int> GetWindowPos() const override { return std::pair<int, int>(m_Data.XPos, m_Data.YPos); }
		std::pair<int, int> GetCursorPos() const override;

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetWindowPosition(int x, int y) override;
		void SetWindowSettings(WindowFlags flags) override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		void SetWindowMode(DWORD newStyle, DWORD newExStyle, int width, int height, int x = 0, int y = 0);
		void SetWindowed();
		void SetBorderless();
		void SetBorderlessFullscreen();
		void SetFullscreenExclusive(int refreshRate = 60);
		void ExitFullscreenExclusive();
		virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_Window;
		Scope<GraphicsContext> m_GraphicsContext;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			int XPos, YPos;
			bool VSync;
			WindowFlags Flags;
			
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}