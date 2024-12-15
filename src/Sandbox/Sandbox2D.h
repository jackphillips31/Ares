#pragma once

#include <Ares.h>
#include <imgui.h>

#include "ui/MainWindow.h"
#include "ui/Performance.h"
#include "ui/WindowSettings.h"
#include "ui/FrameBufferViewer.h"
#include "ui/AssetList.h"

#include "scene/ViewportCamera.h"

class Sandbox2D : public Ares::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Ares::Timestep ts) override;
	void OnRender() override;
	void OnEvent(Ares::Event& e) override;
	virtual void OnImGuiRender() override;

	bool OnWindowFocus(Ares::WindowFocusEvent& event);
	bool OnAssetLoaded(Ares::AssetEvent& event);

private:
	Ares::Window& m_Window;
	ImFont* myFont;

private:
	MainWindowElement m_MainWindowElement;
	PerformanceElement m_PerformanceElement;
	WindowSettingsElement m_WindowSettingsElement;
	FrameBufferViewerElement m_FrameBufferElement;
	AssetListElement m_AssetListElement;

	Ares::Ref<Ares::VertexBuffer> m_VBO;
	Ares::Ref<Ares::IndexBuffer> m_IBO;
	Ares::Ref<Ares::VertexArray> m_VAO;
	Ares::Ref<Ares::VertexShader> m_VertexShader;
	Ares::Ref<Ares::FragmentShader> m_FragmentShader;
	Ares::Ref<Ares::ShaderProgram> m_ShaderProgram;
	Ares::Ref<Ares::FrameBuffer> m_FBO;

	Ares::Scope<ViewportCamera> m_Camera;
};