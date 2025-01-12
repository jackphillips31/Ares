#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS::Components {

	class Light : public Component
	{
	public:
		enum Type : uint8_t
		{
			None = 0,
			Directional,
			Point
		};
	public:
		Light(const Type type = Type::Point);

		// Getters
		Type GetType() const;
		// Color.w is intensity
		glm::vec4 GetColor() const;
		// Properties.x = range
		// Properties.y = falloff
		// Properties.z = inner angle
		// Properties.w = outer angle
		glm::vec4 GetProperties() const;

		// Setters
		void SetType(const Type type);
		void SetColor(const glm::vec3& color);
		void SetIntensity(const float intensity);
		void SetProperties(const glm::vec4& props);
		void SetRange(const float range);
		void SetFalloff(const float falloff);
		void SetInnerAngle(const float innerAngle);
		void SetOuterAngle(const float outerAngle);

	private:
		mutable std::shared_mutex m_Mutex;
		Type m_Type;
		glm::vec4 m_Color;
		glm::vec4 m_Properties;
	};

}