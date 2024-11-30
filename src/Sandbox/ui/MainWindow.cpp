#include <imgui/imgui.h>

#include "ui/MainWindow.h"

MainWindowElement::MainWindowElement()
{
}

void MainWindowElement::Draw()
{
	// Set the window to take up the whole client area
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;


	// Start a fullscreen window
	Ares::Application& app = Ares::Application::Get();
	std::pair<int32_t, int32_t> clientPos = app.GetWindow().GetClientPos();
	ImVec2 clientSize = ImVec2(
		static_cast<float>(app.GetWindow().GetClientWidth()),
		static_cast<float>(app.GetWindow().GetClientHeight())
	);
	ImGui::SetNextWindowPos(ImVec2(
		static_cast<float>(clientPos.first),
		static_cast<float>(clientPos.second)
	));
	ImGui::SetNextWindowSize(clientSize);
	ImGui::Begin("Main Window", nullptr, windowFlags);

	// Menu bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) {}
			if (ImGui::MenuItem("Save")) {}
			if (ImGui::MenuItem("Exit"))
			{
				Ares::WindowCloseEvent event;
				Ares::Application::Get().OnEvent(event);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Add a dock space
	ImGuiID dockspaceId = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspaceId);

	ImGui::End();
}