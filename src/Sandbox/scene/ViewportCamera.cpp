#include "scene/ViewportCamera.h"

ViewportCamera::ViewportCamera(float width, float height)
	: PerspectiveCamera(90.0f, width, height, 0.1f, 100.0f)
{
}

void ViewportCamera::OnUpdate(Ares::Timestep ts)
{
	// Forward movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::W))
	{
		Translate({ 0.0f, 0.0f, -1.0f }, ts, XZ);
	}

	// Backward movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::S))
	{
		Translate({ 0.0f, 0.0f, 1.0f }, ts, XZ);
	}

	// Left movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::A))
	{
		Translate({ -1.0f, 0.0f, 0.0f }, ts, XZ);
	}

	// Right movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::D))
	{
		Translate({ 1.0f, 0.0f, 0.0f }, ts, XZ);
	}

	// Rotate up
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Up))
	{
		RotateLocal({ 1.0f, 0.0f, 0.0f }, ts);
	}

	// Rotate down
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Down))
	{
		RotateLocal({ -1.0f, 0.0f, 0.0f }, ts);
	}

	// Rotate right
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Right))
	{
		RotateGlobal({ 0.0f, -1.0f, 0.0f }, ts);
	}

	// Rotate left
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Left))
	{
		RotateGlobal({ 0.0f, 1.0f, 0.0f }, ts);
	}
}

void ViewportCamera::OnEvent(Ares::Event& e)
{

}