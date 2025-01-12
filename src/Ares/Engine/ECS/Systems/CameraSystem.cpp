#include <arespch.h>
#include "Engine/ECS/Systems/CameraSystem.h"

#include "Engine/Core/Timestep.h"
#include "Engine/ECS/Components/AllComponents.h"
#include "Engine/ECS/Core/EntityManager.h"
#include "Engine/ECS/Core/Scene.h"

namespace Ares::ECS::Systems {

	CameraSystem::CameraSystem()
		: m_ViewportSize(1280.0f, 720.0f), m_ActiveCameraEntityId(0)
	{
	}

	void CameraSystem::OnUpdate(const Scene& scene, const Timestep& timestep)
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);
		EntityManager* entityManager = scene.GetEntityManager();

		// Query entities with Transform and Camera components
		for (auto& entry : entityManager->GetEntityComponents())
		{
			const uint32_t entityId = entry.first;
			Components::Camera* camera = entityManager->GetComponent<Components::Camera>(entityId);
			if (camera != nullptr)
			{
				// Entity has camera - check if active
				if (m_ActiveCameraEntityId == entityId)
				{
					// Update viewport
					camera->SetViewportSize(m_ViewportSize);

					// Update camera position and orientation
					Components::Transform* transform = entityManager->GetComponent<Components::Transform>(entityId);

					if (transform)
					{
						camera->SetPosition(transform->GetPosition());
						camera->SetRotation(transform->GetRotation());
					}
				}
			}
		}
	}

	const uint32_t CameraSystem::GetActiveCameraEntityId()
	{
		std::shared_lock lock(m_Mutex);
		return m_ActiveCameraEntityId;
	}

	const glm::vec2 CameraSystem::GetViewportSize()
	{
		std::shared_lock lock(m_Mutex);
		return m_ViewportSize;
	}

	void CameraSystem::SetActiveCamera(const uint32_t& entityId)
	{
		std::unique_lock lock(m_Mutex);
		m_ActiveCameraEntityId = entityId;
	}

	void CameraSystem::SetActiveCamera(const Entity& entity)
	{
		SetActiveCamera(entity.GetID());
	}

	void CameraSystem::SetViewportSize(const glm::vec2& viewSize)
	{
		{
			std::shared_lock lock(m_Mutex);
			if (m_ViewportSize == viewSize)
				return;
		}
		{
			std::unique_lock lock(m_Mutex);
			m_ViewportSize = viewSize;
		}
	}

}