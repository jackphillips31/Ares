#include <arespch.h>
#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/RendererAPI.h"

namespace Ares {

	namespace Systems {

		Scope<Renderer> Renderer::Create()
		{
			return CreateScope<Renderer>();
		}

		RenderAPI Renderer::GetAPI()
		{
			return Internal::RendererAPI::GetAPI();
		}

		Renderer::Renderer()
			: m_RendererAPI(Internal::RendererAPI::Create()), m_RenderCommand(Internal::RenderCommand::Create(m_RendererAPI.get()))
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

	}

}