#pragma once
#include <Ares.h>

class PerformanceElement : public Ares::ImGuiElement
{
public:
	PerformanceElement();

	void Draw() override;
};