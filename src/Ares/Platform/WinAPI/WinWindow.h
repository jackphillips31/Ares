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

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }
		inline uint32_t GetClientWidth() const override { return m_Data.ClientWidth; }
		inline uint32_t GetClientHeight() const override { return m_Data.ClientHeight; }
		inline std::pair<int32_t, int32_t> GetWindowPos() const override { return std::pair<int32_t, int32_t>(m_Data.XPos, m_Data.YPos); }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetWindowSettings(uint16_t flags) override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		virtual void ApplySettings(uint16_t flags);
		virtual void ConfigureFullscreen(uint16_t flags);
		virtual void ConfigureWindowed(uint16_t flags);

		virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_Window;
		Scope<GraphicsContext> m_GraphicsContext;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			uint32_t ClientWidth, ClientHeight;
			int32_t XPos, YPos;
			bool VSync;
			uint16_t Flags;
			
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}