#pragma once

#include "Engine/Renderer/Scene/Camera.h"

namespace Ares {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane);

		void CalculateProjectionMatrix() override;

	private:
		float m_FieldOfView;
	};

}