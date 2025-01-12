#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS::Components {

	class Camera : public Component
	{
	public:
		enum Mode : uint8_t
		{
			Orthographic,
			Perspective
		};
	public:
		// Constructor
		Camera(Mode mode = Perspective);

		// Getters - Using shared locks for read operations
		Mode GetMode() const;
		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;
		glm::vec2 GetNearFarPlanes() const;
		glm::vec2 GetViewportSize() const;
		float GetOrthoZoom() const;
		float GetPerspectiveFov() const;
		glm::mat4 GetViewProjectionMatrix();

		// Setters - Using unique locks for write operations
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::quat& rotation);
		void SetNearFarPlanes(const glm::vec2& planes);
		void SetViewportSize(const glm::vec2& viewSize);
		void SetOrthoZoom(const float zoom);
		void SetPerspectiveFov(const float fov);

		// Set mode
		void SetOrthographic(
			const glm::vec2& viewSize,
			const float zoom,
			const float nearPlane, const float farPlane
		);
		void SetPerspective(
			const glm::vec2& viewSize,
			const float fov,
			const float nearPlane, const float farPlane
		);

	private:
		void CalculateViewMatrix();
		void CalculateProjectionMatrix();
		void CalculateViewProjectionMatrix();

	private:
		// Mutex for thread safety
		mutable std::shared_mutex m_Mutex;

		// Projection type
		Mode m_Mode;

		// View parameters
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::mat4 m_TranslationMatrix = glm::mat4(1.0f);
		glm::mat4 m_RotationMatrix = glm::mat4(0.0f);

		// Parameters for both types
		float m_NearPlane, m_FarPlane;
		glm::vec2 m_ViewportSize;

		// Orthographic parameters
		float m_OrthoZoom;

		// Perspective parameters
		float m_PerspectiveFov;

		// Matrices
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		// Are matrices dirty
		std::atomic<bool> m_ViewMatrixDirty = true;
		std::atomic<bool> m_ProjectionMatrixDirty = true;
		std::atomic<bool> m_ViewProjectionMatrixDirty = true;
	};

}