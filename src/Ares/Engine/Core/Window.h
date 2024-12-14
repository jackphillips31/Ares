#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/Core.h"
#include "Engine/Core/Flags.h"
#include "Engine/Events/Event.h"
#include "Engine/Renderer/GraphicsContext.h"

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
			uint16_t flags = WindowSettings::DefaultWindow,
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
		virtual void SwapBuffers() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetWindowDimensions() const = 0;
		virtual uint32_t GetClientWidth() const = 0;
		virtual uint32_t GetClientHeight() const = 0;
		virtual glm::uvec2 GetClientDimensions() const = 0;
		virtual glm::ivec2 GetWindowPos() const = 0;
		virtual glm::ivec2 GetClientPos() const = 0;
		virtual uint16_t GetWindowSettings() const = 0;

		// Window attributes
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetWindowPosition(int32_t x, int32_t y) = 0;
		virtual void SetWindowSize(uint32_t width, uint32_t height) = 0;
		virtual void SetClientSize(uint32_t width, uint32_t height) = 0;
		virtual void SetWindowSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClientSizePos(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetWindowSettings(uint16_t flags) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual GraphicsContext* GetGraphicsContext() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}