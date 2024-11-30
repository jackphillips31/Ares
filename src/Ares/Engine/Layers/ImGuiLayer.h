#pragma once

#include "Engine/Core/Layer.h"

namespace Ares {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& name = "ImGuiLayer");
		virtual ~ImGuiLayer() {};

		virtual void Begin() {};
		virtual void End() {};

		static Ref<ImGuiLayer> Create();
	};

}