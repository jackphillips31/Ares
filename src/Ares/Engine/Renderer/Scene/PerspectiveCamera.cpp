#include <arespch.h>

#include "Engine/Renderer/Scene/PerspectiveCamera.h"

namespace Ares {

	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane)
		: m_FieldOfView(fov)
	{
		m_CameraSettings.ViewportDimensions = glm::vec2(width, height);
		m_CameraSettings.NearPlane = nearPlane;
		m_CameraSettings.FarPlane = farPlane;

		m_ProjectionMatrixDirty = true;
	}

	void PerspectiveCamera::CalculateProjectionMatrix()
	{
		if (m_ProjectionMatrixDirty)
		{
			m_ProjectionMatrix = glm::perspective(
				glm::radians(m_FieldOfView),
				m_CameraSettings.ViewportDimensions.x / m_CameraSettings.ViewportDimensions.y,
				m_CameraSettings.NearPlane, m_CameraSettings.FarPlane
			);

			m_ProjectionMatrixDirty = false;
			m_ViewProjectionMatrixDirty = true;
		}
	}

}