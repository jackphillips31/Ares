#include <arespch.h>
#include "Engine/ECS/Components/Transform.h"

namespace Ares::ECS::Components {

	Transform::Transform()
		: m_Position(glm::vec3(0.0f)),
		m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		m_Scale(glm::vec3(1.0f)),
		m_TranslationMatrix(glm::mat4(1.0f)),
		m_RotationMatrix(glm::mat4(1.0f)),
		m_ScaleMatrix(glm::mat4(1.0f)),
		m_TranslationDirty(true),
		m_RotationDirty(true),
		m_ScaleDirty(true),
		m_TransformationDirty(true)
	{
		CalculateTransformationMatrix();
	}

	glm::mat4 Transform::GetTransformationMatrix()
	{
		CalculateTransformationMatrix();

		std::shared_lock lock(m_Mutex);
		return m_TransformationMatrix;
	}

	glm::vec3 Transform::GetPosition() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Position;
	}

	glm::quat Transform::GetRotation() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Rotation;
	}

	glm::vec3 Transform::GetScale() const
	{
		std::shared_lock lock(m_Mutex);
		return m_Scale;
	}

	void Transform::SetTransform(const Transform* transform)
	{
		SetPosition(transform);
		SetRotation(transform);
		SetScale(transform);
	}

	void Transform::SetPosition(const glm::vec3& position)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_Position == position)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_Position = position;
			m_TranslationDirty = true;
		}
	}

	void Transform::SetPosition(const Transform* transform)
	{
		SetPosition(transform->GetPosition());
	}

	void Transform::SetRotation(const glm::quat& rotation)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_Rotation == rotation)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_Rotation = rotation;
			m_RotationDirty = true;
		}
	}

	void Transform::SetRotation(const Transform* transform)
	{
		SetRotation(transform->GetRotation());
	}

	void Transform::SetRotation(const glm::vec3& eulerAngles)
	{
		SetRotation(glm::quat(glm::radians(eulerAngles)));
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_Scale == scale)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_Scale = scale;
			m_ScaleDirty = true;
		}
	}

	void Transform::SetScale(const Transform* transform)
	{
		SetScale(transform->GetScale());
	}

	void Transform::CalculateTranslationMatrix()
	{
		if (m_TranslationDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			m_TranslationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
			m_TransformationDirty = true;
		}
	}

	void Transform::CalculateRotationMatrix()
	{
		if (m_RotationDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			m_RotationMatrix = glm::mat4_cast(m_Rotation);
			m_TransformationDirty = true;
		}
	}

	void Transform::CalculateScaleMatrix()
	{
		if (m_ScaleDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
			m_TransformationDirty = true;
		}
	}

	void Transform::CalculateTransformationMatrix()
	{
		CalculateTranslationMatrix();
		CalculateRotationMatrix();
		CalculateScaleMatrix();

		if (m_TransformationDirty.exchange(false))
		{
			std::unique_lock lock(m_Mutex);
			m_TransformationMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
		}
	}

}