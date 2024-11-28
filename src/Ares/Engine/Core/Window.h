#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Flags.h"
#include "Engine/Events/Event.h"

namespace Ares {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		int32_t XPos;
		int32_t YPos;
		uint16_t Flags;
		void* Icon;

		WindowProps(const std::string& title = "Ares Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			int32_t xpos = 200,
			int32_t ypos = 200,
			uint16_t flags = 0,
			void* icon = nullptr)
			: Title(title), Width(width), Height(height), XPos(xpos), YPos(ypos), Flags(flags), Icon(icon)
		{
		}
	};

	// Interface representing a desktop system based window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetClientWidth() const = 0;
		virtual uint32_t GetClientHeight() const = 0;
		virtual std::pair<int32_t, int32_t> GetWindowPos() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetWindowSettings(uint16_t flags) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}