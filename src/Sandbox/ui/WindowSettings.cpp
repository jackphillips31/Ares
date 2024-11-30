#include "ui/WindowSettings.h"

WindowSettingsElement::WindowSettingsElement()
	: m_Window(Ares::Application::Get().GetWindow()),
	m_WindowSettings(m_Window.GetWindowSettings()),
	m_WindowDimensions{(int32_t)m_Window.GetWidth(), (int32_t)m_Window.GetHeight()},
	m_ClientDimensions{(int32_t)m_Window.GetClientWidth(), (int32_t)m_Window.GetClientHeight()},
	m_WindowPosition{ m_Window.GetWindowPos().first, m_Window.GetWindowPos().second }
{
}

void WindowSettingsElement::Draw()
{
	ImGui::Begin("Window Settings");

	std::bitset<16> binary(m_WindowSettings);
	ImGui::Text("Binary value: %s", binary.to_string().c_str());
	ImGui::Text("Window Dimensions: %u x %u", m_Window.GetWidth(), m_Window.GetHeight());
	ImGui::Text("Client Dimensions: %u x %u", m_Window.GetClientWidth(), m_Window.GetClientHeight());
	std::pair<int32_t, int32_t> windowPos = m_Window.GetWindowPos();
	std::pair<int32_t, int32_t> clientPos = m_Window.GetClientPos();
	ImGui::Text("Window Position: %d x %d", windowPos.first, windowPos.second);
	ImGui::Text("Client Position: %d x %d", clientPos.first, clientPos.second);

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

	ImGui::InputInt2("Window", m_WindowDimensions);
	ImGui::InputInt2("Client", m_ClientDimensions);
	ImGui::InputInt2("Position", m_WindowPosition);

	if (ImGui::Button("Apply Window Size", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		m_Window.SetWindowSize((uint32_t)m_WindowDimensions[0], (uint32_t)m_WindowDimensions[1]);
	}
	if (ImGui::Button("Apply Client Size", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		m_Window.SetClientSize((uint32_t)m_ClientDimensions[0], (uint32_t)m_ClientDimensions[1]);
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
	m_WindowPosition[0] = e.GetXPos();
	m_WindowPosition[1] = e.GetYPos();

	return false;
}

bool WindowSettingsElement::OnWindowResize(Ares::WindowResizeEvent& e)
{
	m_WindowDimensions[0] = (int32_t)e.GetWidth();
	m_WindowDimensions[1] = (int32_t)e.GetHeight();
	m_ClientDimensions[0] = (int32_t)e.GetClientWidth();
	m_ClientDimensions[1] = (int32_t)e.GetClientHeight();

	return false;
}