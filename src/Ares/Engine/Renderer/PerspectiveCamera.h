#pragma once

#include "Engine/Renderer/Camera.h"

namespace Ares {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane);

		glm::mat4 GetProjectionMatrix() const override;

	private:
		float m_FieldOfView;
	};

}