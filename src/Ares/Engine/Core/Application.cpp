#include <arespch.h>
#include "Engine/Core/Application.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Window.h"
#include "Engine/Data/AssetManager.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/ImGui/ImGuiContext.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Data/MemoryManager.h"

namespace Ares {

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const ApplicationSettings& settings)
		: m_Settings(settings), m_Window(nullptr), m_ImGuiContext(nullptr)
	{
		if (s_Instance != nullptr)
		{
			AR_CORE_ASSERT(false, "Application already exists!");
			throw std::exception("Application already exists!");
		}

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
		m_ImGuiContext = ImGuiContext::Create();

		RegisterSystem<Systems::Input>(m_Window.get());
		RegisterSystem<Systems::ThreadPool>(settings.ThreadCount);
		RegisterSystem<Systems::AssetManager>(GetSystem<Systems::ThreadPool>());
		RegisterSystem<Systems::EventQueue>();
		Renderer::Init();

		m_Window->SetEventCallback(
			AR_BIND_SYSTEM_CALLBACK_FN(Systems::EventQueue::Dispatch, GetSystem<Systems::EventQueue>())
		);

		GetSystem<Systems::AssetManager>()->SetEventCallback(
			AR_BIND_SYSTEM_CALLBACK_FN(Systems::EventQueue::Dispatch, GetSystem<Systems::EventQueue>())
		);
		GetSystem<Systems::EventQueue>()->SetEventCallback(AR_BIND_EVENT_FN(Application::OnEvent));
		GetSystem<Systems::EventQueue>()->AddListener<WindowCloseEvent>(AR_BIND_EVENT_FN(Application::OnWindowClose));
		GetSystem<Systems::EventQueue>()->AddListener<WindowResizeEvent>(AR_BIND_EVENT_FN(Application::OnWindowResize));

		Internal::MemoryManager testManager(512);
		//void* data = testManager.Allocate(256);
		void* data1 = testManager.Allocate(64);
		void* data2 = testManager.Allocate(64);
		//void* data3 = testManager.Allocate(512);
		//void* data4 = testManager.Allocate(52);

		testManager.Deallocate(data2);
		testManager.Deallocate(data1);

		void* data5 = testManager.Allocate(16);
		void* data6 = testManager.Allocate(32);

		//testManager.Deallocate(data5);
		//testManager.Deallocate(data6);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		UnregisterSystem<Systems::AssetManager>();
		UnregisterSystem<Systems::ThreadPool>();
		UnregisterSystem<Systems::Input>();
		UnregisterSystem<Systems::EventQueue>();
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

	void Application::PopLayer(Ref<Layer> layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Ref<Layer> overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
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

				if (!m_Minimized)
				{
					for (std::type_index& systemType : m_SystemOrder)
						m_Systems[systemType]->OnUpdate(timestep);

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

			m_ImGuiContext->Begin();
			for (Ref<Layer> layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiContext->End();

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