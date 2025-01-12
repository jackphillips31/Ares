#include <arespch.h>
#include "Engine/Renderer/Buffer.h"

#include "Engine/Data/RawData.h"
#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ares {

	Scope<VertexBuffer> VertexBuffer::Create(const size_t size, const BufferUsage usage)
	{
		return VertexBuffer::Create({ nullptr, size }, usage);
	}

	Scope<VertexBuffer> VertexBuffer::Create(const RawData& data, const BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLVertexBuffer>(data, usage);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<IndexBuffer> IndexBuffer::Create(const size_t size, const BufferUsage usage)
	{
		return IndexBuffer::Create({ nullptr, size }, usage);
	}

	Scope<IndexBuffer> IndexBuffer::Create(const RawData& data, const BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLIndexBuffer>(data, usage);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}