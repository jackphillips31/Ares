#pragma once

#include "Engine/Events/Event.h"

namespace Ares {

	class ImGuiElement
	{
	public:
		virtual ~ImGuiElement() = default;

		virtual void Draw() {};
		virtual void OnUpdate() {};

		virtual void OnEvent(Event& e) {};
	};

}

