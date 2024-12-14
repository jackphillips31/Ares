#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "ui/WindowSettings.h"

WindowSettingsElement::WindowSettingsElement()
	: m_Window(Ares::Application::Get().GetWindow()),
	m_WindowSettings(m_Window.GetWindowSettings()),
	m_WindowDimensions(m_Window.GetWindowDimensions()),
	m_ClientDimensions(m_Window.GetClientDimensions()),
	m_WindowPosition(m_Window.GetWindowPos())
{
}

void WindowSettingsElement::Draw()
{
	ImGui::Begin("Window Settings");

	std::bitset<16> binary(m_WindowSettings);
	glm::uvec2 windowDimensions = m_Window.GetWindowDimensions();
	glm::uvec2 clientDimensions = m_Window.GetClientDimensions();
	ImGui::Text("Binary value: %s", binary.to_string().c_str());
	ImGui::Text("Window Dimensions: %u x %u", windowDimensions.x, windowDimensions.y);
	ImGui::Text("Client Dimensions: %u x %u", clientDimensions.x, clientDimensions.y);
	glm::ivec2 windowPos = m_Window.GetWindowPos();
	glm::ivec2 clientPos = m_Window.GetClientPos();
	ImGui::Text("Window Position: %d x %d", windowPos.x, windowPos.y);
	ImGui::Text("Client Position: %d x %d", clientPos.x, clientPos.y);

	ImGui::SeparatorText("Combined Flags");

	bool isDefaultWindow = m_WindowSettings == AR_WINDOW_DEFAULT_WINDOW;
	if (ImGui::Checkbox("Default Window", &isDefaultWindow))
	{
		m_WindowSettings = isDefaultWindow ? AR_WINDOW_DEFAULT_WINDOW : 0;
	}

	bool isFullscreenBorderless = m_WindowSettings == AR_WINDOW_FULLSCREEN_BORDERLESS;
	if (ImGui::Checkbox("Fullscreen Borderless", &isFullscreenBorderless))
	{
		m_WindowSettings = isFullscreenBorderless ? AR_WINDOW_FULLSCREEN_BORDERLESS : 0;
	}

	bool isFullscreenWindowed = m_WindowSettings == AR_WINDOW_FULLSCREEN_WINDOWED;
	if (ImGui::Checkbox("Fullscreen Windowed", &isFullscreenWindowed))
	{
		m_WindowSettings = isFullscreenWindowed ? AR_WINDOW_FULLSCREEN_WINDOWED : 0;
	}

	ImGui::SeparatorText("Individual Flags");

	bool isWindowed = m_WindowSettings & AR_WINDOW_WINDOWED;
	if (ImGui::Checkbox("Windowed", &isWindowed))
	{
		m_WindowSettings ^= AR_WINDOW_WINDOWED;
	}

	bool isBorderless = m_WindowSettings & AR_WINDOW_BORDERLESS;
	if (ImGui::Checkbox("Borderless", &isBorderless))
	{
		m_WindowSettings ^= AR_WINDOW_BORDERLESS;
	}

	bool isFullscreen = m_WindowSettings & AR_WINDOW_FULLSCREEN;
	if (ImGui::Checkbox("Fullscreen", &isFullscreen))
	{
		m_WindowSettings ^= AR_WINDOW_FULLSCREEN;
	}

	bool isFullscreenEx = m_WindowSettings & AR_WINDOW_FULLSCREEN_EX;
	if (ImGui::Checkbox("Fullscreen Exclusive", &isFullscreenEx))
	{
		m_WindowSettings ^= AR_WINDOW_FULLSCREEN_EX;
	}

	bool isResizable = m_WindowSettings & AR_WINDOW_RESIZABLE;
	if (ImGui::Checkbox("Resizable", &isResizable))
	{
		m_WindowSettings ^= AR_WINDOW_RESIZABLE;
	}

	bool IsMaximized = m_WindowSettings & AR_WINDOW_MAXIMIZED;
	if (ImGui::Checkbox("Maximized", &IsMaximized))
	{
		m_WindowSettings ^= AR_WINDOW_MAXIMIZED;
	}

	ImGui::Separator();

	if (ImGui::Button("Set Window Settings", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		m_Window.SetWindowSettings(m_WindowSettings);
	}

	ImGui::SeparatorText("Size & Position");

	ImGui::InputInt2("Window", reinterpret_cast<int*>(glm::value_ptr(m_WindowDimensions)));
	ImGui::InputInt2("Client", reinterpret_cast<int*>(glm::value_ptr(m_ClientDimensions)));
	ImGui::InputInt2("Position", glm::value_ptr(m_WindowPosition));

	if (ImGui::Button("Apply Window Size", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		m_Window.SetWindowSize(static_cast<uint32_t>(m_WindowDimensions[0]), static_cast<uint32_t>(m_WindowDimensions[1]));
	}
	if (ImGui::Button("Apply Client Size", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		m_Window.SetClientSize(static_cast<uint32_t>(m_ClientDimensions[0]), static_cast<uint32_t>(m_ClientDimensions[1]));
	}
	if (ImGui::Button("Apply Window Position", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		m_Window.SetWindowPosition(m_WindowPosition[0], m_WindowPosition[1]);
	}

	ImGui::End();
}

void WindowSettingsElement::OnEvent(Ares::Event& e)
{
	Ares::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Ares::WindowMovedEvent>(AR_BIND_EVENT_FN(WindowSettingsElement::OnWindowMoved));
	dispatcher.Dispatch<Ares::WindowResizeEvent>(AR_BIND_EVENT_FN(WindowSettingsElement::OnWindowResize));
}

bool WindowSettingsElement::OnWindowMoved(Ares::WindowMovedEvent& e)
{
	m_WindowPosition = glm::ivec2(e.GetXPos(), e.GetYPos());

	return false;
}

bool WindowSettingsElement::OnWindowResize(Ares::WindowResizeEvent& e)
{
	m_WindowDimensions = glm::uvec2(e.GetWidth(), e.GetHeight());
	m_ClientDimensions = glm::uvec2(e.GetClientWidth(), e.GetClientHeight());

	return false;
}