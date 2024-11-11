#include "Sandbox2D.h"
#include <imgui/imgui.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Ares::Application::Get().GetWindow())
{
}

void Sandbox2D::OnAttach()
{
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
	ImGui::Begin("Test");
	ImGui::LabelText("TEST LABEL", "TESTLABEL2");
	ImGui::End();
}