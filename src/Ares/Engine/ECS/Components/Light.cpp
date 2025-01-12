#include <arespch.h>
#include "Engine/ECS/Components/Light.h"

namespace Ares::ECS::Components {

	Light::Light(const Type type)
		: m_Type(type), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Properties(20.0f, 15.0f, 0.0f, 0.0f)
	{
	}

	Light::Type Light::GetType() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Type;
	}

	glm::vec4 Light::GetColor() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Color;
	}

	glm::vec4 Light::GetProperties() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Properties;
	}

	void Light::SetType(const Type type)
	{
		std::unique_lock lock(m_Mutex);
		m_Type = type;
	}

	void Light::SetColor(const glm::vec3& color)
	{
		std::unique_lock lock(m_Mutex);
		m_Color = glm::vec4(color, m_Color.w);
	}

	void Light::SetIntensity(const float intensity)
	{
		std::unique_lock lock(m_Mutex);
		m_Color.w = intensity;
	}

	void Light::SetProperties(const glm::vec4& props)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties = props;
	}

	void Light::SetRange(const float range)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties.x = range;
	}

	void Light::SetFalloff(const float falloff)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties.y = falloff;
	}

	void Light::SetInnerAngle(const float innerAngle)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties.z = innerAngle;
	}

	void Light::SetOuterAngle(const float outerAngle)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties.w = outerAngle;
	}

}