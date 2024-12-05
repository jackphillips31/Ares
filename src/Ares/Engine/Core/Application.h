#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Flags.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Window.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Layers/ImGuiLayer.h"

int EntryPoint(int argc, char** argv);

namespace Ares {

	struct ApplicationSettings
	{
		std::string Name = "Ares Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		uint32_t UpdatesPerSecond = 120;

		uint16_t WindowStyle = WindowSettings::DefaultWindow;
		void* Icon = nullptr;

		ApplicationSettings(
			const std::string& name = "Ares Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			uint16_t windowStyle = WindowSettings::DefaultWindow,
			void* icon = nullptr
		) : Name(name), Width(width), Height(height), WindowStyle(windowStyle), Icon(icon)
		{
		}
	};

	class Application
	{
	public:
		Application(const ApplicationSettings& settings = ApplicationSettings());
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	
	private:
		Scope<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		bool m_ShowConsole = false;
		LayerStack m_LayerStack;
		double m_UpdatesPerSecond = 0.0f;
		double m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;

		friend int ::EntryPoint(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}