#include <arespch.h>
#include "Engine/Renderer/RendererAPI.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ares::Internal {

	RenderAPI RendererAPI::s_API = RenderAPI::None;

	Scope<RendererAPI> RendererAPI::Create(const RenderAPI api)
	{
		if (s_API != RenderAPI::None && s_API != api)
		{
			AR_CORE_WARN("RendererAPI is being created with a different renderer!");
		}

		s_API = api;

		switch (api)
		{
		case RenderAPI::None: AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}