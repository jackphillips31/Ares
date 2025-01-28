#pragma once
#include "Engine/ImGui/ImGuiContext.h"

namespace Ares {

	class OpenGLImGuiContext : public ImGuiContext
	{
	public:
		OpenGLImGuiContext();
		~OpenGLImGuiContext();

		void Begin() override;
		void End() override;
	};

}