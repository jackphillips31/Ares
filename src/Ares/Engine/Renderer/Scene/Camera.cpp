#include <arespch.h>

#include "Engine/Renderer/Scene/Camera.h"

namespace Ares {

	glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		const_cast<Camera*>(this)->CalculateViewProjectionMatrix();

		return m_ViewProjectionMatrix;
	}

	void Camera::RotateLocal(const glm::vec3& degrees)
	{
		glm::quat deltaRotation = glm::quat(glm::radians(degrees));
		m_Rotation = glm::normalize(deltaRotation * m_Rotation);

		m_ViewMatrixDirty = true;
	}

	void Camera::RotateGlobal(const glm::vec3& degrees)
	{
		glm::quat deltaRotation = glm::quat(glm::radians(degrees));
		m_Rotation = glm::normalize(m_Rotation * deltaRotation);

		m_ViewMatrixDirty = true;
	}

	void Camera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_ProjectionMatrixDirty = true;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;

		m_ViewMatrixDirty = true;
	}

	void Camera::SetRotation(const glm::vec3& degrees)
	{
		m_Rotation = glm::quat(glm::radians(degrees));

		m_ViewMatrixDirty = true;
	}

	void Camera::CalculateViewMatrix()
	{
		if (m_ViewMatrixDirty)
		{
			// Translate to the position
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_Position);

			// Rotate according to the quaternion
			glm::mat4 rotation = glm::toMat4(glm::conjugate(m_Rotation));

			// Combine rotation and translation
			m_ViewMatrix = rotation * translation;

			m_ViewMatrixDirty = false;
			m_ViewProjectionMatrixDirty = true;
		}
	}

	void Camera::CalculateViewProjectionMatrix()
	{
		CalculateViewMatrix();
		CalculateProjectionMatrix();

		if (m_ViewProjectionMatrixDirty)
		{
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
			m_ViewProjectionMatrixDirty = false;
		}
	}

}