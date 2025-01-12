#include <arespch.h>
#include "Engine/Renderer/UniformBuffer.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Ares {

	Scope<UniformBuffer> UniformBuffer::Create(const size_t size, const uint32_t bindingPoint, const BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLUniformBuffer>(size, bindingPoint, usage);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<UniformBuffer> UniformBuffer::Create(const RawData& data, const uint32_t bindingPoint, const BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLUniformBuffer>(data, bindingPoint, usage);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}