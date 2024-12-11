#include <arespch.h>

#include "Engine/Renderer/Renderer.h"

#ifdef AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinOpenGLContext.h"
#endif

#include "Engine/Renderer/GraphicsContext.h"

namespace Ares {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: {
			#ifdef AR_PLATFORM_WINDOWS
				return CreateScope<WinOpenGLContext>(window);
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