#include "scene/ViewportCamera.h"

ViewportCamera::ViewportCamera(float width, float height)
	: PerspectiveCamera(90.0f, width, height, 0.1f, 100.0f),
	m_TempViewportWidth(width),
	m_TempViewportHeight(height)
{
}

void ViewportCamera::OnUpdate(Ares::Timestep ts)
{
	float timestep = static_cast<float>(ts);

	// Forward movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Up))
	{
		glm::vec3 forward = glm::rotate(m_Rotation, { 0.0f, 0.0f, -1.0f });

		// Constrain to X/Z plane
		forward.y = 0.0f;
		forward = glm::normalize(forward);
		m_Position += forward * m_MovementSpeed * timestep;

		m_ViewMatrixDirty = true;
	}

	// Backward movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Down))
	{
		glm::vec3 backward = glm::rotate(m_Rotation, { 0.0f, 0.0f, 1.0f });

		// Constrain to X/Z plane
		backward.y = 0.0f;
		backward = glm::normalize(backward);
		m_Position += backward * m_MovementSpeed * timestep;

		m_ViewMatrixDirty = true;
	}

	// Left movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Left))
	{
		glm::vec3 left = glm::rotate(m_Rotation, { -1.0f, 0.0f, 0.0f });

		// Constrain to X/Z plane
		left.y = 0.0f;
		left = glm::normalize(left);
		m_Position += left * m_MovementSpeed * timestep;

		m_ViewMatrixDirty = true;
	}

	// Right movement
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Right))
	{
		glm::vec3 right = glm::rotate(m_Rotation, { 1.0f, 0.0f, 0.0f });

		// Constrain to X/Z plane
		right.y = 0.0f;
		right = glm::normalize(right);
		m_Position += right * m_MovementSpeed * timestep;

		m_ViewMatrixDirty = true;
	}

	// Rotate right
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::D))
	{
		RotateLocal(glm::vec3(0.0f, -m_RotationSpeed * timestep, 0.0f));
	}

	// Rotate left
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::A))
	{
		RotateLocal(glm::vec3(0.0f, m_RotationSpeed * timestep, 0.0f));
	}

	// Update viewport size if changed
	if (m_ViewportWidth != m_TempViewportWidth || m_ViewportHeight != m_TempViewportHeight)
	{
		if (m_TempViewportWidth && m_TempViewportHeight)
			SetViewportSize(m_TempViewportWidth, m_TempViewportHeight);
	}
}

void ViewportCamera::OnEvent(Ares::Event& e)
{

}

void ViewportCamera::SetTempDimensions(float width, float height)
{
	m_TempViewportWidth = width;
	m_TempViewportHeight = height;
}