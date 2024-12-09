#include <arespch.h>

#include "Engine/Renderer/Scene/Camera.h"

namespace Ares {

	glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		const_cast<Camera*>(this)->CalculateViewProjectionMatrix();

		return m_ViewProjectionMatrix;
	}

	void Camera::Translate(const glm::vec3& direction, float amount, PlaneLock planeLock)
	{
		glm::vec3 newDirection = glm::rotate(m_Rotation, direction);

		switch (planeLock)
		{
		case PlaneLock::XY: {
			newDirection.z = 0.0f;
			glm::normalize(newDirection);
			break;
		}
		case PlaneLock::XZ: {
			newDirection.y = 0.0f;
			glm::normalize(newDirection);
			break;
		}
		case PlaneLock::YZ: {
			newDirection.x = 0.0f;
			glm::normalize(newDirection);
			break;
		}
		default: {
			glm::normalize(newDirection);
			break;
		}
		}

		m_Position += newDirection * amount;
		m_ViewMatrixDirty = true;
	}

	void Camera::Translate(const glm::vec3& direction, Timestep ts, PlaneLock planeLock)
	{
		Translate(direction, m_CameraSettings.MovementSpeed * static_cast<float>(ts), planeLock);
	}

	void Camera::RotateLocal(const glm::vec3& degrees)
	{
		glm::quat deltaRotation = glm::quat(glm::radians(degrees));
		m_Rotation = glm::normalize(m_Rotation * deltaRotation);

		m_ViewMatrixDirty = true;
	}

	void Camera::RotateLocal(const glm::vec3& degrees, Timestep ts)
	{
		RotateLocal(degrees * m_CameraSettings.RotationSpeed * static_cast<float>(ts));
	}

	void Camera::RotateGlobal(const glm::vec3& degrees)
	{
		glm::quat deltaRotation = glm::quat(glm::radians(degrees));
		m_Rotation = glm::normalize(deltaRotation * m_Rotation);

		m_ViewMatrixDirty = true;
	}

	void Camera::RotateGlobal(const glm::vec3& degrees, Timestep ts)
	{
		RotateGlobal(degrees * m_CameraSettings.RotationSpeed * static_cast<float>(ts));
	}

	void Camera::SetViewportSize(const glm::vec2& dimensions)
	{
		if (m_CameraSettings.ViewportDimensions != dimensions)
		{
			m_CameraSettings.ViewportDimensions = dimensions;

			m_ProjectionMatrixDirty = true;
		}
	}

	void Camera::SetNearFarPlanes(float nearPlane, float farPlane)
	{
		if (m_CameraSettings.NearPlane != nearPlane || m_CameraSettings.FarPlane != farPlane)
		{
			m_CameraSettings.NearPlane = nearPlane;
			m_CameraSettings.FarPlane = farPlane;

			m_ProjectionMatrixDirty = true;
		}
	}

	void Camera::SetMovementSpeed(float speed)
	{
		m_CameraSettings.MovementSpeed = speed;
	}

	void Camera::SetRotationSpeed(float speed)
	{
		m_CameraSettings.RotationSpeed = speed;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		if (m_Position != position)
		{
			m_Position = position;

			m_ViewMatrixDirty = true;
		}
	}

	void Camera::SetRotation(const glm::vec3& degrees)
	{
		glm::quat newRotation = glm::quat(glm::radians(degrees));
		if (m_Rotation != newRotation)
		{
			m_Rotation = newRotation;

			m_ViewMatrixDirty = true;
		}
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