#include <arespch.h>
#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Ares {

	Scope<FrameBuffer> FrameBuffer::Create(const uint32_t width, const uint32_t height)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None: AR_CORE_ASSERT(false, "Cannot create framebuffer if RenderAPI::None"); return nullptr;
		case RenderAPI::OpenGL: return CreateScope<OpenGLFrameBuffer>(width, height);
		}

		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}