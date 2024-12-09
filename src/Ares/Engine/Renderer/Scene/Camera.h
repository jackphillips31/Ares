#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class Camera
	{
	public:
		enum PlaneLock
		{
			None = 0,
			XY,
			XZ,
			YZ
		};

	public:
		virtual ~Camera() = default;

		glm::mat4 GetViewProjectionMatrix() const;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		void Translate(const glm::vec3& direction, float amount, PlaneLock planeLock = PlaneLock::None);
		void Translate(const glm::vec3& direction, Timestep ts, PlaneLock planeLock = PlaneLock::None);
		void RotateLocal(const glm::vec3& degrees);
		void RotateLocal(const glm::vec3& degrees, Timestep ts);
		void RotateGlobal(const glm::vec3& degrees);
		void RotateGlobal(const glm::vec3& degrees, Timestep ts);

		void SetViewportSize(const glm::vec2& dimensions);
		void SetNearFarPlanes(float nearPlane, float farPlane);
		void SetMovementSpeed(float speed);
		void SetRotationSpeed(float speed);
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& degrees);

	protected:
		Camera() = default;

		void CalculateViewMatrix();
		void CalculateViewProjectionMatrix();
		virtual void CalculateProjectionMatrix() {}

	protected:
		struct Settings
		{
			float NearPlane = 0.1f;										// If changed, set m_ProjectionMatrixDirty = true
			float FarPlane = 100.0f;									// v
			glm::vec2 ViewportDimensions = glm::vec2(1280.0f, 720.0f);	// v
			float MovementSpeed = 2.0f;
			float RotationSpeed = 60.0f;

			Settings()
				: NearPlane(0.1f), FarPlane(100.0f), ViewportDimensions(glm::vec2(1280.0f, 720.0f)), MovementSpeed(2.0f), RotationSpeed(60.0f)
			{
			}
		};

	protected:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };					// If changed, set m_ViewMatrixDirty = true
		glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);		// v

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		bool m_ViewMatrixDirty = true;
		bool m_ProjectionMatrixDirty = true;
		bool m_ViewProjectionMatrixDirty = true;

		Settings m_CameraSettings;
	};

}