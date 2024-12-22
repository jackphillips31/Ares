#pragma once

#include <glm/gtx/quaternion.hpp>

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS {

	namespace Components {

		struct Orientation : public glm::quat, public Component
		{
			// Inherit constructors from glm::quat
			using glm::quat::quat;

			// Default constructor
			Orientation() : glm::quat(1.0f, 1.0f, 1.0f, 1.0f) {}

			// Constructor from Euler angles
			Orientation(const glm::vec3& eulerAngles)
				: glm::quat(glm::radians(eulerAngles)) {}
			Orientation(float x, float y, float z)
				: glm::quat(glm::radians(glm::vec3(x, y, z))) {}

			// Read x, y, z as Euler angles
			const float x() const
			{
				return glm::degrees(glm::eulerAngles(*this)).x;
			}
			const float y() const
			{
				return glm::degrees(glm::eulerAngles(*this)).y;
			}
			const float z() const
			{
				return glm::degrees(glm::eulerAngles(*this)).z;
			}
			const glm::vec3 GetEulerAngles() const
			{
				return glm::degrees(glm::eulerAngles(*this));
			}
		};

	}

}