#include <arespch.h>

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RendererAPI.h"

#ifdef AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinOpenGLSharedContext.h"
#endif

#include "Engine/Renderer/SharedGraphicsContext.h"

namespace Ares {

	Scope<SharedGraphicsContext> SharedGraphicsContext::Create(const void* primaryContext)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: {
		#ifdef AR_PLATFORM_WINDOWS
			return CreateScope<WinOpenGLSharedContext>(primaryContext);
		#else
			AR_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
		}
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}