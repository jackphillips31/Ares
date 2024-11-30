#pragma once
#include <Ares.h>
#include <imgui/imgui.h>

class WindowSettingsElement
{
public:
	WindowSettingsElement();
	~WindowSettingsElement() = default;

	void Draw();

	void OnEvent(Ares::Event& e);

private:
	bool OnWindowMoved(Ares::WindowMovedEvent& e);
	bool OnWindowResize(Ares::WindowResizeEvent& e);

private:
	Ares::Window& m_Window;
	uint16_t m_WindowSettings;
	int32_t m_WindowDimensions[2];
	int32_t m_ClientDimensions[2];
	int32_t m_WindowPosition[2];
};