#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS::Components {

	class Transform : public Component
	{
	public:
		Transform();

		// Getters
		glm::mat4 GetTransformationMatrix();
		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;
		glm::vec3 GetScale() const;

		// Setters
		void SetTransform(const Transform* transform);
		void SetPosition(const glm::vec3& position);
		void SetPosition(const Transform* transform);
		void SetRotation(const glm::quat& rotation);
		void SetRotation(const Transform* transform);
		void SetRotation(const glm::vec3& eulerAngles);
		void SetScale(const glm::vec3& scale);
		void SetScale(const Transform* transform);

	private:
		void CalculateTranslationMatrix();
		void CalculateRotationMatrix();
		void CalculateScaleMatrix();
		void CalculateTransformationMatrix();

	private:
		mutable std::shared_mutex m_Mutex;
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 m_TranslationMatrix;
		glm::mat4 m_RotationMatrix;
		glm::mat4 m_ScaleMatrix;
		glm::mat4 m_TransformationMatrix;
		std::atomic<bool> m_TranslationDirty;
		std::atomic<bool> m_RotationDirty;
		std::atomic<bool> m_ScaleDirty;
		std::atomic<bool> m_TransformationDirty;
	};

}