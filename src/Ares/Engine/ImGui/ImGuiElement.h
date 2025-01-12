#pragma once

namespace Ares {

	class Event;
	class Timestep;

	class ImGuiElement
	{
	public:
		virtual ~ImGuiElement() = default;

		virtual void Draw() {};
		virtual void OnUpdate(const Timestep& ts) {};

		virtual void OnEvent(Event& e) {};
	};

}