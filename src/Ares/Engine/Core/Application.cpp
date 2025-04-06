#include <arespch.h>
#include "Engine/Core/Application.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/MainThreadQueue.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Window.h"
#include "Engine/Data/AssetManager.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/ImGui/ImGuiContext.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Data/MemoryManager/MemoryPool.h"

namespace Ares {

	Application* Application::s_Instance = nullptr;
	std::shared_mutex Application::s_StaticMutex;
	
	Application::Application(const ApplicationSettings& settings)
		: m_MemoryManager(), m_Settings(settings), m_Window(nullptr), m_ImGuiContext(nullptr),
		m_LayerStack(nullptr),
		m_Running(true),
		m_Systems(*(m_MemoryManager.GetDefaultAllocator())),
		m_SystemOrder(*(m_MemoryManager.GetDefaultAllocator()))
	{
		if (s_Instance != nullptr)
		{
			AR_CORE_ASSERT(false, "Application already exists!");
			throw std::runtime_error("Application already exists!");
		}

		s_Instance = this;

		WindowProps windowProps = WindowProps(
			settings.Name,
			settings.Width,
			settings.Height,
			200,
			200,
			settings.WindowStyle,
			settings.Icon
		);

		Systems::Renderer::SetAPI(settings.Renderer);

		m_LayerStack = LayerStack::Create();
		m_Window = Window::Create(windowProps);
		m_ImGuiContext = ImGuiContext::Create();

		RegisterSystem<Systems::Input>(m_Window.get());
		RegisterSystem<Systems::ThreadPool>(settings.ThreadCount);
		RegisterSystem<Systems::MainThreadQueue>();
		RegisterSystem<Systems::AssetManager>(GetSystem<Systems::ThreadPool>(), GetSystem<Systems::MainThreadQueue>());
		RegisterSystem<Systems::EventQueue>();
		RegisterSystem<Systems::Renderer>();

		m_Window->SetEventCallback(
			AR_BIND_SYSTEM_CALLBACK_FN(Systems::EventQueue::Dispatch, GetSystem<Systems::EventQueue>())
		);

		GetSystem<Systems::AssetManager>()->SetEventCallback(
			AR_BIND_SYSTEM_CALLBACK_FN(Systems::EventQueue::Dispatch, GetSystem<Systems::EventQueue>())
		);
		GetSystem<Systems::EventQueue>()->SetEventCallback(AR_BIND_EVENT_FN(Application::OnEvent));
		GetSystem<Systems::EventQueue>()->AddListener<WindowCloseEvent>(AR_BIND_EVENT_FN(Application::OnWindowClose));
		GetSystem<Systems::EventQueue>()->AddListener<WindowResizeEvent>(AR_BIND_EVENT_FN(Application::OnWindowResize));
	}

	Application::~Application()
	{
		UnregisterSystem<Systems::Renderer>();
		UnregisterSystem<Systems::AssetManager>();
		UnregisterSystem<Systems::MainThreadQueue>();
		UnregisterSystem<Systems::ThreadPool>();
		UnregisterSystem<Systems::Input>();
		UnregisterSystem<Systems::EventQueue>();
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		{
			std::unique_lock lock(m_LayerStackMutex);
			m_LayerStack->PushLayer(layer);
		}
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay)
	{
		{
			std::unique_lock lock(m_LayerStackMutex);
			m_LayerStack->PushOverlay(overlay);
		}
		overlay->OnAttach();
	}

	void Application::PopLayer(Ref<Layer> layer)
	{
		{
			std::unique_lock lock(m_LayerStackMutex);
			m_LayerStack->PopLayer(layer);
		}
		layer->OnDetach();
	}

	void Application::PopOverlay(Ref<Layer> overlay)
	{
		{
			std::unique_lock lock(m_LayerStackMutex);
			m_LayerStack->PopOverlay(overlay);
		}
		overlay->OnDetach();
	}

	void Application::Run()
	{
		AR_CORE_INFO("Engine Running...");
		m_Running = true;

		UpdateLoop();
		RenderLoop();
	}

	void Application::UpdateLoop()
	{
		GetSystem<Systems::ThreadPool>()->SubmitTask([this]()
			{
				m_LastUpdateTime = std::chrono::high_resolution_clock::now();
				std::chrono::nanoseconds targetInterval(1000000000 / m_Settings.UpdatesPerSecond);
				TimePoint nextUpdateTime = m_LastUpdateTime;

				while (m_Running.load())
				{
					Timestep timestep = std::chrono::duration<double>(nextUpdateTime.time_since_epoch() - m_LastUpdateTime.time_since_epoch()).count();
					{
						std::shared_lock lock(m_SystemMutex);
						for (const std::type_index& element : m_SystemOrder)
						{
							if (const auto& it = m_Systems.find(element); it != m_Systems.end())
							{
								it->second->OnUpdate(timestep);
							}
						}
					}
					
					{
						std::shared_lock lock(m_LayerStackMutex);
						for (Ref<Layer>& entry : *m_LayerStack)
							entry->OnUpdate(timestep);
					}

					m_LastUpdateTime = nextUpdateTime;
					nextUpdateTime += targetInterval;
					std::this_thread::sleep_until(nextUpdateTime);
				}
			});
	}

	void Application::RenderLoop()
	{
		Systems::MainThreadQueue* mainThreadQueue = GetSystem<Systems::MainThreadQueue>();
		Systems::Renderer* renderer = GetSystem<Systems::Renderer>();
		m_LastRenderTime = std::chrono::high_resolution_clock::now();

		while (m_Running.load())
		{
			m_Window->OnUpdate();

			if (m_Minimized)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			TimePoint currentTime = std::chrono::high_resolution_clock::now();
			Timestep timestep = std::chrono::duration<double>(currentTime.time_since_epoch() - m_LastRenderTime.time_since_epoch()).count();

			{
				std::shared_lock lock(m_LayerStackMutex);
				for (Ref<Layer>& entry : *m_LayerStack)
				{
					entry->OnRender();
				}

				{
					std::shared_lock lock2(m_SystemMutex);
					for (const std::type_index& element : m_SystemOrder)
					{
						if (const auto& it = m_Systems.find(element); it != m_Systems.end())
						{
							it->second->OnRender();
						}
					}
				}

				m_ImGuiContext->Begin();
				for (Ref<Layer>& entry : *m_LayerStack)
				{
					entry->OnImGuiRender();
				}
				m_ImGuiContext->End();
			}

			m_Window->SwapBuffers();
			m_LastRenderTime = currentTime;
		}
	}

	void Application::OnEvent(Event& e)
	{
		std::shared_lock lock(m_LayerStackMutex);
		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
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
		GetSystem<Systems::Renderer>()->OnClientResize(e.GetClientWidth(), e.GetClientHeight());

		return false;
	}

}