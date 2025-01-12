#include "ui/Performance.h"

#include <imgui.h>

PerformanceElement::PerformanceElement()
{
}

void PerformanceElement::Draw()
{
	ImGui::Begin("Performance");
	ImGui::Text("Frame time: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}