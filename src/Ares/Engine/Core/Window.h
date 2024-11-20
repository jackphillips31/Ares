#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Flags.h"
#include "Engine/Events/Event.h"

namespace Ares {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		unsigned int XPos;
		unsigned int YPos;
		WindowFlags Flags;

		WindowProps(const std::string& title = "Ares Engine",
			unsigned int width = 1280,
			unsigned int height = 720,
			unsigned int xpos = 200,
			unsigned int ypos = 200,
			WindowFlags flags = WindowFlags::None)
			: Title(title), Width(width), Height(height), XPos(xpos), YPos(ypos), Flags(flags)
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

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual std::pair<int, int> GetWindowPos() const = 0;
		virtual std::pair<int, int> GetCursorPos() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetWindowPosition(int x, int y) = 0;
		virtual void SetWindowSettings(WindowFlags flags) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}