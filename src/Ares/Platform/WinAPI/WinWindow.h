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
		void SwapBuffers() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }
		inline uint32_t GetClientWidth() const override { return m_Data.ClientWidth; }
		inline uint32_t GetClientHeight() const override { return m_Data.ClientHeight; }
		inline std::pair<int32_t, int32_t> GetWindowPos() const override { return std::pair<int32_t, int32_t>(m_Data.XPos, m_Data.YPos); }
		inline std::pair<int32_t, int32_t> GetClientPos() const override { return std::pair<int32_t, int32_t>(m_Data.ClientXPos, m_Data.ClientYPos); }
		inline uint16_t GetWindowSettings() const override { return m_Data.Flags; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline void SetWindowPosition(int32_t x, int32_t y) override { SetWindowSizePos(x, y, m_Data.Width, m_Data.Height); }
		inline void SetWindowSize(uint32_t width, uint32_t height) override { SetWindowSizePos(m_Data.XPos, m_Data.YPos, width, height); }
		inline void SetClientSize(uint32_t width, uint32_t height) override { SetClientSizePos(m_Data.XPos, m_Data.YPos, width, height); }
		void SetWindowSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height) override;
		void SetClientSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height) override;
		void SetWindowSettings(uint16_t flags) override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }
		inline virtual GraphicsContext* GetGraphicsContext() const override { return m_GraphicsContext.get(); }

	private:
		void Init(const WindowProps& props);
		void Shutdown();
		void ApplySettings(uint16_t flags);
		void ConfigureFullscreen(uint16_t flags);
		void ConfigureWindowed(uint16_t flags);

		LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_Window;
		Scope<GraphicsContext> m_GraphicsContext;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			uint32_t ClientWidth, ClientHeight;
			int32_t XPos, YPos, ClientXPos, ClientYPos;
			bool VSync;
			uint16_t Flags;
			DEVMODE OriginalDisplay;
			
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}