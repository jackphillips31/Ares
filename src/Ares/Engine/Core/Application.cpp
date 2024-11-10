#include <arespch.h>
#include "Engine/Core/Application.h"

#include "Engine/Core/Timestep.h"

#include <GLFW/glfw3.h>

namespace Ares {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = Window::Create();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Window->OnUpdate();
		}
	}
}