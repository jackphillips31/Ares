#pragma once

#include <Ares.h>
#include <imgui.h>

class PerformanceElement : public Ares::ImGuiElement
{
public:
	PerformanceElement();

	void Draw() override;

private:
	ImFont* myFont = nullptr;
};