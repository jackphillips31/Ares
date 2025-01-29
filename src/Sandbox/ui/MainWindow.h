#pragma once

#include <Ares.h>

class MainWindowElement : public Ares::ImGuiElement
{
public:
	MainWindowElement(Ares::Application& app);

	void Draw() override;

private:
	Ares::Systems::EventQueue* m_EventSystem;
};