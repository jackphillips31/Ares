#include <arespch.h>

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Engine/Renderer/Texture.h"

namespace Ares {

	Ref<Texture2D> Texture2D::Create(const std::string& name, const FileBuffer& fileBuffer)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(name, fileBuffer);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}