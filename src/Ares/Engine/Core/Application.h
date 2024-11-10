#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Core/Window.h"

int main(int argc, char** argv);

namespace Ares {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		void Run();
	
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}