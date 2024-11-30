#pragma once

#include "Engine/Events/Event.h"

namespace Ares {

	class ImGuiElement
	{
	public:
		virtual ~ImGuiElement() = default;

		virtual void Draw() {};
		virtual void BeginDraw() {};
		virtual void EndDraw() {};

		virtual void OnEvent(Event& e) {};
	};

}

