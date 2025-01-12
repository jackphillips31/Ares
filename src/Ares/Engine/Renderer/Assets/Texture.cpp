#include <arespch.h>
#include "Engine/Renderer/Assets/Texture.h"

#include "Engine/Data/RawData.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Ares {

	Scope<Texture> Texture::Create(const std::string& name, const RawData& data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLTexture>(name, data);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<Texture> Texture::Create(const std::string& name, const glm::uvec2& dimensions, const RawData& rawData, const Format format)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLTexture>(name, dimensions, rawData, format);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}