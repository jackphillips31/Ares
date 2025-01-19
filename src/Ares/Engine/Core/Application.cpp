#include <arespch.h>
#include "Engine/Core/Application.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/MainThreadQueue.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Window.h"
#include "Engine/Data/AssetManager.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Layers/ImGuiLayer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Ares {

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const ApplicationSettings& settings)
		: m_Settings(settings)
	{
		s_Instance = this;

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

		ThreadPool::Init(settings.ThreadCount);
		EventQueue::Init();
		AssetManager::Init();
		Renderer::Init();
		MainThreadQueue::Init();

		EventQueue::SetEventCallback(AR_BIND_EVENT_FN(Application::OnEvent));
		EventQueue::AddListener<WindowCloseEvent>(AR_BIND_EVENT_FN(Application::OnWindowClose));
		EventQueue::AddListener<WindowResizeEvent>(AR_BIND_EVENT_FN(Application::OnWindowResize));

		m_ImGuiLayer = ImGuiLayer::Create();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MainThreadQueue::Shutdown();
		Renderer::Shutdown();
		Input::Shutdown();
		AssetManager::Shutdown();
		EventQueue::Shutdown();
		ThreadPool::Shutdown();
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

			while (currentTime - m_LastFrameTime >= (1.0f / static_cast<double>(m_Settings.UpdatesPerSecond)))
			{
				Timestep timestep = currentTime - m_LastFrameTime;
				m_LastFrameTime = currentTime;

				m_Window->OnUpdate();
				AssetManager::OnUpdate();
				MainThreadQueue::OnUpdate();

				if (!m_Minimized)
				{
					for (Ref<Layer> layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				EventQueue::OnUpdate();
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
		Renderer::OnClientResize(e.GetClientWidth(), e.GetClientHeight());

		return false;
	}

}