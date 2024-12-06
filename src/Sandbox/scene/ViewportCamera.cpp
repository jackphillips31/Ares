#include <glm/common.hpp>
#include <glm/gtx/euler_angles.hpp>

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

	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Left))
		m_Orientation.y += m_RotationSpeed * timestep;
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Right))
		m_Orientation.y -= m_RotationSpeed * timestep;
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Up))
		m_Orientation.x -= m_RotationSpeed * timestep;
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::Down))
		m_Orientation.x += m_RotationSpeed * timestep;

	if (Ares::Input::IsKeyPressed(Ares::KeyCode::A))
		m_Position -= GetRightVector() * m_MovementSpeed * timestep;
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::D))
		m_Position += GetRightVector() * m_MovementSpeed * timestep;

	if (Ares::Input::IsKeyPressed(Ares::KeyCode::W))
		m_Position.z += m_MovementSpeed * timestep;
	if (Ares::Input::IsKeyPressed(Ares::KeyCode::S))
		m_Position.z -= m_MovementSpeed * timestep;

	// Clamp pitch to prevent flipping
	m_Orientation.x = glm::clamp(m_Orientation.x, -89.0f, 89.0f);

	// Wrap yaw and roll to keep them within 0-360 degrees
	m_Orientation.y = fmod(m_Orientation.y, 360.0f);
	m_Orientation.z = fmod(m_Orientation.z, 360.0f);

	// Update viewport size if changed
	if (m_ViewportWidth != m_TempViewportWidth || m_ViewportHeight != m_TempViewportHeight)
	{
		AR_TRACE("VIEWPORT CHANGED SIZE!");
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

glm::vec3 ViewportCamera::GetForwardVector() const
{
	glm::vec3 radians = glm::radians(m_Orientation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(radians.y, radians.x, radians.z);
	return glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

glm::vec3 ViewportCamera::GetRightVector() const
{
	glm::vec3 radians = glm::radians(m_Orientation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(radians.y, radians.x, radians.z);
	return glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
}

glm::vec3 ViewportCamera::GetUpVector() const
{
	glm::vec3 radians = glm::radians(m_Orientation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(radians.y, radians.x, radians.z);
	return glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
}