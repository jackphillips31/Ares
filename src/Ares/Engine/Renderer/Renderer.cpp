#include <arespch.h>

#include "Engine/Renderer/Renderer.h"

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

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

}