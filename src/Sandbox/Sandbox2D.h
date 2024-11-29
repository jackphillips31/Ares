#pragma once

#include <Ares.h>
#include <imgui/imgui.h>

class Sandbox2D : public Ares::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Ares::Timestep ts) override;
	void OnEvent(Ares::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	Ares::Window& m_Window;
	ImFont* myFont = nullptr;
	Ares::Ref<Ares::FrameBuffer> m_FrameBuffer;
};