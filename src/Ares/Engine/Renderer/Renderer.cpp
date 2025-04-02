#include <arespch.h>
#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/RenderCommandQueue.h"
#include "Engine/Renderer/RendererAPI.h"

namespace Ares {

	namespace Systems {

		Scope<Renderer> Renderer::Create(const RenderAPI api)
		{
			return CreateScope<Renderer>(api);
		}

		void Renderer::OnRender()
		{
			m_CommandQueue->ExecuteCommands();
		}

		RenderAPI Renderer::GetAPI()
		{
			return Internal::RendererAPI::GetAPI();
		}

		void Renderer::SetAPI(const RenderAPI api)
		{
			Internal::RendererAPI::SetAPI(api);
		}

		Renderer::Renderer(const RenderAPI api)
			: m_RendererAPI(Internal::RendererAPI::Create(api)),
			m_RenderCommand(Internal::RenderCommand::Create(m_RendererAPI.get())),
			m_CommandQueue(Internal::RenderCommandQueue::Create(m_RendererAPI.get()))
		{
			AR_CORE_INFO("Initializing System: Renderer");
			m_RendererAPI->Init();
		}

		Renderer::~Renderer()
		{
			m_RenderCommand.reset();
			m_RendererAPI.reset();
		}

		void Renderer::OnClientResize(const uint32_t width, const uint32_t height)
		{
			m_RenderCommand->SetViewport(0, 0, width, height);
		}

		Internal::RenderCommand* Renderer::RenderCommand()
		{
			return m_RenderCommand.get();
		}

		Internal::RenderCommandQueue* Renderer::RenderCommandQueue()
		{
			return m_CommandQueue.get();
		}

	}

}