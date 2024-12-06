#include <arespch.h>
#include <glm/gtx/euler_angles.hpp>

#include "Engine/Renderer/Camera.h"

namespace Ares {

	Camera::Camera(
		const glm::vec3& position,
		const glm::vec3& orientation,
		float nearPlane, float farPlane,
		float viewportWidth, float viewportHeight
	) : m_Position(position), m_Orientation(orientation),
		m_NearPlane(nearPlane), m_FarPlane(farPlane),
		m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight)
	{
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		glm::vec3 radians = glm::radians(m_Orientation);
		glm::mat4 rotationMatrix = glm::yawPitchRoll(radians.y, radians.x, radians.z);
		glm::vec3 forward = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
		glm::vec3 up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

		return glm::lookAt(m_Position, m_Position + forward, up);
	}

	glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = static_cast<float>(width);
		m_ViewportHeight = static_cast<float>(height);
	}

	void Camera::Serialize(std::ostream& os) const
	{
		os << m_Position.x << " " << m_Position.y << " " << m_Position.z << "\n";
		os << m_Orientation.x << " " << m_Orientation.y << " " << m_Orientation.z << "\n";
		os << m_MovementSpeed << " " << m_RotationSpeed << "\n";
	}

	void Camera::Deserialize(std::istream& is)
	{
		is >> m_Position.x >> m_Position.y >> m_Position.z;
		is >> m_Orientation.x >> m_Orientation.y >> m_Orientation.z;
		is >> m_MovementSpeed >> m_RotationSpeed;
	}

}