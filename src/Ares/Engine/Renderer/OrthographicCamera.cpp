#include <arespch.h>
//#include <glm/ext/matrix_clip_space.hpp>

#include "Engine/Renderer/OrthographicCamera.h"

/*
namespace Ares {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
		: Camera(glm::vec3(0.0f), glm::vec3(0.0f), nearPlane, farPlane, right - left, top - bottom),
		m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
	{
	}

	glm::mat4 OrthographicCamera::GetProjectionMatrix() const
	{
		return glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Left = left;
		m_Right = right;
		m_Bottom = bottom;
		m_Top = top;
	}

	void OrthographicCamera::Serialize(std::ostream& os) const
	{
		Camera::Serialize(os);
		os << m_Left << " " << m_Right << " " << m_Bottom << " " << m_Top << "\n";
	}

	void OrthographicCamera::Deserialize(std::istream& is)
	{
		Camera::Deserialize(is);
		is >> m_Left >> m_Right >> m_Bottom >> m_Top;
	}

}
*/