#include "scene/OrthoTestCamera.h"

OrthoTestCamera::OrthoTestCamera(float width, float height)
	: OrthographicCamera(width, height)
{
}

void OrthoTestCamera::OnUpdate(Ares::Timestep ts)
{
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::A))
	{
		Translate({ -1.0f, 0.0f, 0.0f }, ts);
	}
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::D))
	{
		Translate({ 1.0f, 0.0f, 0.0f }, ts);
	}
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::W))
	{
		Translate({ 0.0f, 1.0f, 0.0f }, ts);
	}
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::S))
	{
		Translate({ 0.0f, -1.0f, 0.0f }, ts);
	}
}

void OrthoTestCamera::OnEvent(Ares::Event& e)
{
	Ares::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Ares::MouseScrolledEvent>(AR_BIND_EVENT_FN(OrthoTestCamera::OnMouseScroll));
}

bool OrthoTestCamera::OnMouseScroll(Ares::MouseScrolledEvent& e)
{
	m_Zoom += -e.GetYOffset() / 10;
	m_ProjectionMatrixDirty = true;
	return false;
}