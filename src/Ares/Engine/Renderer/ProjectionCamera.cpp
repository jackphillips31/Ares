#include <arespch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Renderer/PerspectiveCamera.h"

namespace Ares {

	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane)
		: Camera(glm::vec3(0.0f), glm::vec3(0.0f), nearPlane, farPlane, width, height),
		m_FieldOfView(fov)
	{
	}

	glm::mat4 PerspectiveCamera::GetProjectionMatrix() const
	{
		return glm::perspective(
			glm::radians(m_FieldOfView),
			static_cast<float>(m_ViewportWidth / m_ViewportHeight),
			m_NearPlane, m_FarPlane
		);
	}

}