#pragma once

#include <Ares.h>

class WindowSettingsElement : public Ares::ImGuiElement
{
public:
	WindowSettingsElement();

	void Draw() override;
	void OnEvent(Ares::Event& e) override;

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