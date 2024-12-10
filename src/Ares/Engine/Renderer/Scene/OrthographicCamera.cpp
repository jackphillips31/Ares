#include <arespch.h>

#include "Engine/Renderer/Scene/OrthographicCamera.h"

namespace Ares {

	OrthographicCamera::OrthographicCamera(float width, float height, float nearPlane, float farPlane)
		: m_Zoom(1.0f)
	{
		m_CameraSettings.ViewportDimensions = glm::vec2(width, height);
		m_CameraSettings.NearPlane = nearPlane;
		m_CameraSettings.FarPlane = farPlane;

		m_ProjectionMatrixDirty = true;
	}

	void OrthographicCamera::CalculateProjectionMatrix()
	{
		if (m_ProjectionMatrixDirty)
		{
			float aspectRatio = m_CameraSettings.ViewportDimensions.x / m_CameraSettings.ViewportDimensions.y;

			m_ProjectionMatrix = glm::ortho(
				-aspectRatio * m_Zoom,
				aspectRatio * m_Zoom,
				-m_Zoom,
				m_Zoom,
				m_CameraSettings.NearPlane,
				m_CameraSettings.FarPlane
			);

			m_ProjectionMatrixDirty = false;
			m_ViewProjectionMatrixDirty = true;
		}
	}

}