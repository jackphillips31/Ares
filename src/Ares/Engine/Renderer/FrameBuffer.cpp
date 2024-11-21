#include <arespch.h>

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include "Engine/Renderer/FrameBuffer.h"

namespace Ares {

	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: AR_CORE_ASSERT(false, "Cannot create framebuffer if RendererAPI::None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(width, height);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}