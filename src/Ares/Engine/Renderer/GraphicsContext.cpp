#include <arespch.h>
#include "Engine/Renderer/GraphicsContext.h"

#include "Engine/Renderer/Renderer.h"

#if AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinOpenGLContext.h"
#endif

namespace Ares {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None: AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:
		{
			#if AR_PLATFORM_WINDOWS
				return CreateScope<WinOpenGLContext>(window);
			#else
				AR_CORE_ASSERT(false, "Unknown platform!");
				return nullptr;
			#endif
			}
		}

		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}