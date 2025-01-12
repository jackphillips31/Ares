#pragma once
#include <Ares.h>
#include <imgui.h>

#include "ui/MainWindow.h"
#include "ui/Performance.h"
#include "ui/WindowSettings.h"
#include "ui/FrameBufferViewer.h"
#include "ui/AssetList.h"
#include "ui/Editor/EntityList.h"

class Sandbox2D : public Ares::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(const Ares::Timestep& ts) override;
	void OnRender() override;
	void OnEvent(Ares::Event& e) override;
	virtual void OnImGuiRender() override;

	bool OnWindowFocus(Ares::WindowFocusEvent& event);
	void CreateLightEntity();
	void CreateCameraEntity();
	void CreateQuadMeshEntities();
	void CreateTeapotEntity();
	void LoadShaderProgram();
	void LoadDefaultTexture();

private:
	Ares::Window& m_Window;
	ImFont* myFont;

private:
	MainWindowElement m_MainWindowElement;
	PerformanceElement m_PerformanceElement;
	WindowSettingsElement m_WindowSettingsElement;
	FrameBufferViewerElement m_FrameBufferElement;
	AssetListElement m_AssetListElement;
	EntityListElement m_EntityListElement;

	Ares::Scope<Ares::ECS::Scene> m_SandboxScene;
	Ares::ECS::Entity m_LightEntity;
	Ares::ECS::Entity m_CameraEntity;
	Ares::ECS::Entity m_SquareEntity;
	Ares::ECS::Entity m_SquareEntity2;
	Ares::ECS::Entity m_SquareEntity3;
	Ares::ECS::Entity m_TeapotEntity;
};