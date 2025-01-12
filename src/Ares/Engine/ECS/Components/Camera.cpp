#include <arespch.h>
#include "Engine/ECS/Components/Camera.h"

#include "Engine/ECS/Components/Transform.h"

namespace Ares::ECS::Components {

	Camera::Camera(Mode mode)
		: m_Mode(mode),
		m_ViewportSize(1280.0f, 720.0f),
		m_OrthoZoom(1.0f),
		m_PerspectiveFov(90.0f),
		m_NearPlane(0.1f), m_FarPlane(100.0f)
	{
		CalculateViewProjectionMatrix();
	}

	Camera::Mode Camera::GetMode() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Mode;
	}

	glm::vec3 Camera::GetPosition() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Position;
	}

	glm::quat Camera::GetRotation() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Rotation;
	}

	glm::vec2 Camera::GetNearFarPlanes() const
	{
		std::shared_lock lock(m_Mutex);
		return glm::vec2(m_NearPlane, m_FarPlane);
	}

	glm::vec2 Camera::GetViewportSize() const
	{
		std::shared_lock lock(m_Mutex);
		return m_ViewportSize;
	}

	float Camera::GetOrthoZoom() const
	{
		std::shared_lock lock(m_Mutex);
		return m_OrthoZoom;
	}

	float Camera::GetPerspectiveFov() const
	{
		std::shared_lock lock(m_Mutex);
		return m_PerspectiveFov;
	}

	glm::mat4 Camera::GetViewProjectionMatrix()
	{
		CalculateViewProjectionMatrix();
		std::shared_lock lock(m_Mutex);
		return m_ViewProjectionMatrix;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_Position == position)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_Position = position;
			m_ViewMatrixDirty = true;
		}
	}

	void Camera::SetRotation(const glm::quat& rotation)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_Rotation == rotation)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_Rotation = rotation;
			m_ViewMatrixDirty = true;
		}
	}

	void Camera::SetNearFarPlanes(const glm::vec2& planes)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_NearPlane == planes.x && m_FarPlane == planes.y)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_NearPlane = planes.x;
			m_FarPlane = planes.y;
			m_ProjectionMatrixDirty = true;
		}
	}

	void Camera::SetViewportSize(const glm::vec2& viewSize)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_ViewportSize == viewSize)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_ViewportSize = viewSize;
			m_ProjectionMatrixDirty = true;
		}
	}

	void Camera::SetOrthoZoom(const float zoom)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_OrthoZoom == zoom)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_OrthoZoom = zoom;
			m_ProjectionMatrixDirty = true;
		}
	}

	void Camera::SetPerspectiveFov(const float fov)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_PerspectiveFov == fov)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_PerspectiveFov = fov;
			m_ProjectionMatrixDirty = true;
		}
	}

	void Camera::SetOrthographic(
		const glm::vec2& viewSize,
		const float zoom,
		const float nearPlane, const float farPlane
	)
	{
		std::unique_lock lock(m_Mutex);
		m_ViewportSize = viewSize;
		m_OrthoZoom = zoom;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_ProjectionMatrixDirty = true;
		m_Mode = Mode::Orthographic;
	}

	void Camera::SetPerspective(
		const glm::vec2& viewSize,
		const float fov,
		const float nearPlane, const float farPlane
	)
	{
		std::unique_lock lock(m_Mutex);
		m_ViewportSize = viewSize;
		m_PerspectiveFov = fov;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_ProjectionMatrixDirty = true;
		m_Mode = Mode::Perspective;
	}

	void Camera::CalculateViewMatrix()
	{
		if (m_ViewMatrixDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			glm::mat4 translation;
			glm::mat4 rotation;

			translation = glm::translate(glm::mat4(1.0f), -m_Position);
			rotation = glm::mat4_cast(glm::conjugate(m_Rotation));

			m_ViewMatrix = rotation * translation;
			m_ViewProjectionMatrixDirty = true;
		}
	}

	void Camera::CalculateProjectionMatrix()
	{
		if (m_ProjectionMatrixDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			if (m_Mode == Mode::Orthographic)
			{
				float aspectRatio = m_ViewportSize.x / m_ViewportSize.y;

				m_ProjectionMatrix = glm::ortho(
					-aspectRatio * m_OrthoZoom,
					aspectRatio * m_OrthoZoom,
					-m_OrthoZoom, m_OrthoZoom,
					m_NearPlane, m_FarPlane
				);
			}
			else if (m_Mode == Mode::Perspective)
			{
				m_ProjectionMatrix = glm::perspective(
					glm::radians(m_PerspectiveFov),
					m_ViewportSize.x / m_ViewportSize.y,
					m_NearPlane, m_FarPlane
				);
			}
			m_ViewProjectionMatrixDirty = true;
		}
	}

	void Camera::CalculateViewProjectionMatrix()
	{
		CalculateViewMatrix();
		CalculateProjectionMatrix();

		if (m_ViewProjectionMatrixDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}
}