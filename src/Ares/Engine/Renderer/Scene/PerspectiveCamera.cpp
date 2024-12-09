#include <arespch.h>

#include "Engine/Renderer/Scene/PerspectiveCamera.h"

namespace Ares {

	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane)
		: m_FieldOfView(fov)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;

		m_ProjectionMatrixDirty = true;
	}

	void PerspectiveCamera::CalculateProjectionMatrix()
	{
		if (m_ProjectionMatrixDirty)
		{
			m_ProjectionMatrix = glm::perspective(
				glm::radians(m_FieldOfView),
				m_ViewportWidth / m_ViewportHeight,
				m_NearPlane, m_FarPlane
			);

			m_ProjectionMatrixDirty = false;
			m_ViewProjectionMatrixDirty = true;
		}
	}

}