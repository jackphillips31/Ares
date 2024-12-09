#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		glm::mat4 GetViewProjectionMatrix() const;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		void RotateLocal(const glm::vec3& degrees);
		void RotateGlobal(const glm::vec3& degrees);

		void SetViewportSize(float width, float height);
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& degrees);

	protected:
		Camera() = default;

		void CalculateViewMatrix();
		void CalculateViewProjectionMatrix();
		virtual void CalculateProjectionMatrix() {}

	protected:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };				// If changed, set m_ViewMatrixDirty = true
		glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);	// v
		float m_NearPlane = 0.1f;									// If changed, set m_ProjectionMatrixDirty = true
		float m_FarPlane = 100.0f;									// v
		float m_ViewportWidth = 1280.0f;							// v
		float m_ViewportHeight = 720.0f;							// v

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		bool m_ViewMatrixDirty = true;
		bool m_ProjectionMatrixDirty = true;
		bool m_ViewProjectionMatrixDirty = true;
	};

}