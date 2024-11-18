#pragma once

#include <imgui.h>

#include "Engine/Core/Layer.h"
#include "Engine/Core/Window.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Ares {

	class TitleBar : public Layer
	{
	public:
		TitleBar(const std::string& title);
		virtual ~TitleBar() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnEvent(Event& e) override;
		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseMove(MouseMovedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

	private:
		std::string m_Title;
		unsigned int m_Width;
		unsigned int m_Height;
		Window& m_WindowRef;

		bool m_Dragging = false;
		ImVec2 m_LastMousePos = { 0.0f, 0.0f };
		ImFont* myFont = nullptr;
	};

}