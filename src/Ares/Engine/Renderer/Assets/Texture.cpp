#include <arespch.h>
#include "Engine/Renderer/Assets/Texture.h"

#include "Engine/Data/RawData.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Ares {

	Scope<Texture> Texture::Create(const String& name, const RawData& data)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateScope<OpenGLTexture>(name, data);
		}

		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Scope<Texture> Texture::Create(const String& name, const glm::uvec2& dimensions, const RawData& rawData, const Format format)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateScope<OpenGLTexture>(name, dimensions, rawData, format);
		}

		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}