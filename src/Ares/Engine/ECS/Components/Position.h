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

			// Parameterized constructor
			Position(float x, float y, float z) : glm::vec3(x, y, z) {}

			// Copy assignment & copy constructor
			Position& operator=(const glm::vec3& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
				return *this;
			}
			Position(const Position& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
			}
		};

	}

}