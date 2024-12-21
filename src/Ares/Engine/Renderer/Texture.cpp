#include <arespch.h>

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Engine/Renderer/Texture.h"

namespace Ares {

	Ref<Texture> Texture::Create(const std::string& name, const FileBuffer& fileBuffer)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture>(name, fileBuffer);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture> Texture::Create(const std::string& name, const RawData& data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture>(name, data);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture> Texture::Create(const std::string& name, const glm::uvec2& dimensions, Format format)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture>(name, dimensions, format);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}