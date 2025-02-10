#include <arespch.h>
#include "Engine/ImGui/ImGuiContext.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLImGuiContext.h"

namespace Ares {

	Scope<ImGuiContext> ImGuiContext::Create()
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::OpenGL: return CreateScope<OpenGLImGuiContext>();
		case RenderAPI::None: AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		}
	
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}