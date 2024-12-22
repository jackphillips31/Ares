#pragma once

#include <glm/glm.hpp>

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS {

	namespace Components {

		struct Position : public glm::vec3, public Component
		{
			// Inherit constructors from glm::vec3
			using glm::vec3::vec3;

			// Default constructor
			Position() : glm::vec3(0.0f, 0.0f, 0.0f) {}

			// Constructor from vec3
			Position(const glm::vec3 position) : glm::vec3(position) {}

			// Parameterized constructor
			Position(float x, float y, float z) : glm::vec3(x, y, z) {}

			// Setters
			void SetPosition(const glm::vec3& pos)
			{
				*this = Position(pos);
			}
			void SetPosition(float x, float y, float z)
			{
				SetPosition({ x, y, z });
			}
		};

	}

}