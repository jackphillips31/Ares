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

	struct StartupSettings
	{
		uint16_t WindowSettings = 0;
	};

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		static void SetStartupSettings(uint16_t flags = 0);

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		bool m_ShowConsole = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		static StartupSettings s_Settings;

		friend int ::EntryPoint(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}