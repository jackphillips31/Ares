#include <arespch.h>
#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/RenderCommand.h"

namespace Ares {

	void Renderer::Init()
	{
		AR_CORE_INFO("Initializing Renderer");

		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
		RenderCommand::Shutdown();
	}

	void Renderer::OnClientResize(const uint32_t width, const uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

}