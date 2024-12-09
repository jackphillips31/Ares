#pragma once

#include "Engine/Renderer/Scene/Camera.h"
/*
namespace Ares {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

		glm::mat4 GetProjectionMatrix() const override;

		void SetProjection(float left, float right, float bottom, float top);

		void Serialize(std::ostream& os) const;
		void Deserialize(std::istream& is);

	private:
		float m_Left, m_Right;
		float m_Bottom, m_Top;
	};

}
*/