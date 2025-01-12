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
	glm::uvec2 m_WindowDimensions;
	glm::uvec2 m_ClientDimensions;
	glm::ivec2 m_WindowPosition;
};