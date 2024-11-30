#include "ui/MainWindow.h"

#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Ares::Application::Get().GetWindow()), myFont(nullptr)
{
}

void Sandbox2D::OnAttach()
{
	//const char* fontPath = "assets/fonts/Inter_18pt-SemiBold.ttf";
	const char* fontPath = "assets/fonts/Inter_18pt-Regular.ttf";
	float fontSize = 14.0f;

	ImFontConfig config;
	config.PixelSnapH = true;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.FontGlobalScale = 1.0f;
	io.Fonts->AddFontDefault();
	myFont = io.Fonts->AddFontFromFileTTF(fontPath, fontSize, &config);
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
	m_WindowSettingsElement.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::PushFont(myFont);

	m_MainWindowElement.Draw();

	m_PerformanceElement.Draw();

	m_WindowSettingsElement.Draw();

	m_FrameBufferElement.BeginDraw();

	ImVec2 availableSize = ImGui::GetContentRegionAvail();
	Ares::RenderCommand::SetViewport(0, 0, static_cast<uint32_t>(availableSize.x), static_cast<uint32_t>(availableSize.y));
	Ares::RenderCommand::SetClearColor({ 1.0f, 0.1f, 0.1f, 0.1f });
	Ares::RenderCommand::Clear();
	
	m_FrameBufferElement.EndDraw();

	ImGui::ShowDemoWindow();

	Ares::Log::GetConsole()->Draw("Console", true);

	ImGui::PopFont();
}