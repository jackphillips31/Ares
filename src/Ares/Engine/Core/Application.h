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

		uint16_t WindowStyle = WindowSettings::DefaultWindow;
		void* Icon = nullptr;
	};

	class Application
	{
	public:
		Application(ApplicationSettings& settings);
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

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

		friend int ::EntryPoint(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}