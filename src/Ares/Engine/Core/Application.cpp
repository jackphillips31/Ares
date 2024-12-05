#include <arespch.h>

#include "Engine/Core/Input.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Core/Application.h"

namespace Ares {

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const ApplicationSettings& settings)
	{
		s_Instance = this;

		m_UpdatesPerSecond = static_cast<double>(settings.UpdatesPerSecond);
		m_LastFrameTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		WindowProps windowProps = WindowProps(
			settings.Name,
			settings.Width,
			settings.Height,
			200,
			200,
			settings.WindowStyle,
			settings.Icon
		);

		m_Window = Window::Create(windowProps);
		m_Window->SetEventCallback(AR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = ImGuiLayer::Create();
		PushOverlay(m_ImGuiLayer);	
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		Input::Shutdown();
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		AR_CORE_INFO("Engine Running...");
		while (m_Running)
		{
			double currentTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

			while (currentTime - m_LastFrameTime >= (1.0f / m_UpdatesPerSecond))
			{
				Timestep timestep = currentTime - m_LastFrameTime;
				m_LastFrameTime = currentTime;

				m_Window->OnUpdate();

				if (!m_Minimized)
				{
					for (Ref<Layer> layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}

			if (m_Minimized)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			if (!m_Running)
				break;

			for (Ref<Layer> layer : m_LayerStack)
				layer->OnRender();

			m_ImGuiLayer->Begin();
			for (Ref<Layer> layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->SwapBuffers();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AR_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(AR_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(AR_BIND_EVENT_FN(Application::OnKeyPressed));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetClientWidth(), e.GetClientHeight());

		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

}