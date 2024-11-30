#include "ui/Performance.h"

PerformanceElement::PerformanceElement()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.Fonts->Fonts.Size > 1)
	{
		myFont = io.Fonts->Fonts[1];
	}
}

void PerformanceElement::Draw()
{
	if (!myFont)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.Fonts->Fonts.Size > 1)
			myFont = io.Fonts->Fonts[1];
	}

	if (myFont)
		ImGui::PushFont(myFont);

	ImGui::Begin("Performance");
	ImGui::Text("Frame time: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	if (myFont)
		ImGui::PopFont();
}