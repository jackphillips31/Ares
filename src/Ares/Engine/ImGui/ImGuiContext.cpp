#include <arespch.h>
#include "Engine/ImGui/ImGuiContext.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLImGuiContext.h"

namespace Ares {

	Scope<ImGuiContext> ImGuiContext::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return CreateScope<OpenGLImGuiContext>();
		case RendererAPI::API::None: AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		}
	
		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}