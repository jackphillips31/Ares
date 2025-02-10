#include <arespch.h>
#include "Engine/Renderer/VertexArray.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ares {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateRef<OpenGLVertexArray>();
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}