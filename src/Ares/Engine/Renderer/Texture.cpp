#include <arespch.h>

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Engine/Renderer/Texture.h"

namespace Ares {

	Ref<Texture2D> Texture2D::Create(const std::string& name, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: AR_CORE_ASSERT(false, "Cannot create Texture2D if RendererAPI::None"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(name, width, height);
		}
	}

}