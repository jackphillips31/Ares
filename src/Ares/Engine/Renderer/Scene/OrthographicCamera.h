#pragma once

#include "Engine/Renderer/Scene/Camera.h"

namespace Ares {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float width, float height, float nearPlane = -1.0f, float farPlane = 1.0f);

		void CalculateProjectionMatrix() override;

	protected:
		float m_Zoom;
	};

}