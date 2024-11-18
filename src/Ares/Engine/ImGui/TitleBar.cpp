#include <arespch.h>
#include <imgui.h>

#include "Engine/Core/Application.h"
#include "Engine/ImGui/Style.h"

#include "Engine/ImGui/TitleBar.h"

namespace Ares {

	TitleBar::TitleBar(const std::string& title)
		: Layer("TitleBar"),
		m_Title(title), m_Width(0), m_Height(30), m_WindowRef(Application::Get().GetWindow())
	{
	}

	void TitleBar::OnAttach()
	{
		Application& app = Application::Get();
		m_Width = app.GetWindow().GetWidth();
	}

	void TitleBar::OnDetach()
	{
	}

	void TitleBar::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(AR_BIND_EVENT_FN(TitleBar::OnWindowResize));
		dispatcher.Dispatch<MouseMovedEvent>(AR_BIND_EVENT_FN(TitleBar::OnMouseMove));
		dispatcher.Dispatch<MouseButtonPressedEvent>(AR_BIND_EVENT_FN(TitleBar::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(AR_BIND_EVENT_FN(TitleBar::OnMouseButtonReleased));
	}

	void TitleBar::OnUpdate(Timestep ts)
	{
	}

	void TitleBar::OnImGuiRender()
	{
		if (myFont == nullptr)
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			myFont = io.Fonts->Fonts[1];
		}
		std::pair<int, int> windowPos = m_WindowRef.GetWindowPos();

		ImGui::SetNextWindowPos(ImVec2(windowPos.first, windowPos.second), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_Always);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("##TitleBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);

		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(windowPos.first, windowPos.second),
			ImVec2(windowPos.first + m_Width, windowPos.second + m_Height),
			Style::Colors::HeaderBG
		);

		ImGui::SetCursorPos(ImVec2(10, 7));
		ImGui::PushFont(myFont);
		ImGui::TextUnformatted(m_Title.c_str());

		ImGui::PushStyleColor(ImGuiCol_Button, Style::Colors::HeaderBG);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Style::Colors::HeaderBG_Hover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Style::Colors::HeaderBG_Active);
		ImGui::SetCursorPos(ImVec2(m_Width - m_Height, 0));
		if (ImGui::Button("X", ImVec2(m_Height, m_Height)))
		{
			AR_CORE_WARN("QUIT BUTTON PRESSED");
			PostQuitMessage(0);
		}

		ImGui::PopStyleColor(3);
		ImGui::PopFont();
		ImGui::PopStyleVar(2);

		ImGui::End();
	}

	bool TitleBar::OnWindowResize(WindowResizeEvent& e)
	{
		AR_CORE_INFO("TitleBar OnWindowResize!!");
		m_Width = e.GetWidth();
		return false;
	}

	bool TitleBar::OnMouseMove(MouseMovedEvent& e)
	{
		std::pair<int, int> currentMousePos = m_WindowRef.GetCursorPos();
		if (m_Dragging)
		{
			ImVec2 delta = { currentMousePos.first - m_LastMousePos.x, currentMousePos.second - m_LastMousePos.y };
			std::pair<int, int> windowPos = m_WindowRef.GetWindowPos();
			m_WindowRef.SetPos(windowPos.first + (int)delta.x, windowPos.second + (int)delta.y);	
		}
		m_LastMousePos = { (float)currentMousePos.first, (float)currentMousePos.second };

		return false;
	}

	bool TitleBar::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		int windowWidth = m_WindowRef.GetWidth();
		std::pair<int, int> windowPos = m_WindowRef.GetWindowPos();
		std::pair<int, int> mousePos = m_WindowRef.GetCursorPos();

		bool isMouseOverTitle = false;

		if (mousePos.second > windowPos.second && mousePos.second < windowPos.second + m_Height && mousePos.first > windowPos.first && mousePos.first < windowPos.first + windowWidth)
		{
			isMouseOverTitle = true;
		}

		if (e.GetMouseButton() == AR_MOUSE_BUTTON_0 && isMouseOverTitle)
		{
			m_Dragging = true;
		}

		return false;
	}

	bool TitleBar::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == AR_MOUSE_BUTTON_0)
		{
			m_Dragging = false;
		}

		return false;
	}

}