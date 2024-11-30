#pragma once

#include <Ares.h>

class MainWindowElement : public Ares::ImGuiElement
{
public:
	MainWindowElement();

	void Draw() override;
};