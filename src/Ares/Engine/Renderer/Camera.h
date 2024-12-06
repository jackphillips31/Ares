#pragma once
#include <glm/glm.hpp>

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual glm::mat4 GetProjectionMatrix() const = 0;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetViewProjectionMatrix() const;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		void SetViewportSize(uint32_t width, uint32_t height);
		void Serialize(std::ostream& os) const;
		void Deserialize(std::istream& is);

		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetOrientation(const glm::vec3 orientation) { m_Orientation = orientation; }
		inline void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
		inline void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }

	protected:
		Camera(
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
			const glm::vec3& orientation = glm::vec3(0.0f, 0.0f, 0.0f),
			float nearPlane = 0.1f, float farPlane = 100.0f,
			float viewportWidth = 1280, float viewportHeight = 720
		);

	protected:
		glm::vec3 m_Position;
		glm::vec3 m_Orientation;
		float m_MovementSpeed;
		float m_RotationSpeed;
		float m_NearPlane;
		float m_FarPlane;
		float m_ViewportWidth;
		float m_ViewportHeight;
	};

}