#include <arespch.h>
#include "Engine/Renderer/RendererAPI.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ares::Internal {

	RenderAPI RendererAPI::s_API = RenderAPI::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RenderAPI::None: AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}