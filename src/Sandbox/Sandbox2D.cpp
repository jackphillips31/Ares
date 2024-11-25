#include "imgui/WindowSettings.h"

#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Ares::Application::Get().GetWindow())
{
}

void Sandbox2D::OnAttach()
{
	const char* fontPath = "assets/fonts/Inter_18pt-SemiBold.ttf";
	float fontSize = 14.0f;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	myFont = io.Fonts->AddFontFromFileTTF(fontPath, fontSize);

	io.Fonts->Build();
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Ares::Timestep ts)
{
	Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Ares::RenderCommand::Clear();
}

void Sandbox2D::OnEvent(Ares::Event& e)
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::PushFont(myFont);
	ImGui::Begin("Performance");
	ImGui::Text("Frame time: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::PopFont();

	DrawWindowSettingsWindow();

	ImGui::ShowDemoWindow();

	Ares::Log::GetConsole()->Draw("Console", true);
}