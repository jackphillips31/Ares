#include <arespch.h>

#include "Engine/ECS/Components/AllComponents.h"

#include "Engine/ECS/Systems/CameraSystem.h"

namespace Ares::ECS::Systems {

	void CameraSystem::OnUpdate(Scene& scene, Timestep timestep)
	{
		EntityManager* entityManager = scene.GetEntityManager();
		Components::Camera* activeCamera = nullptr;

		// Query entities with Position and Camera components
		for (auto& entry : entityManager->GetEntityMap())
		{
			const uint32_t entityId = entry.first;
			Components::Camera* camera = entityManager->GetComponent<Components::Camera>(entityId);
			if (camera != nullptr)
			{
				// Entity has camera - check if active
				if (camera->IsActive())
				{
					activeCamera = camera;

					// Update viewport
					camera->SetViewportSize(m_ViewWidth, m_ViewHeight);

					// Update camera position and orientation
					Components::Position* position = entityManager->GetComponent<Components::Position>(entityId);
					Components::Orientation* orientation = entityManager->GetComponent<Components::Orientation>(entityId);

					if (position)
						camera->SetPosition(*position);
					if (orientation)
						camera->SetOrientation(*orientation);
				}
			}
		}
	}

	void CameraSystem::SetViewportSize(const uint32_t& width, const uint32_t& height)
	{
		m_ViewWidth = width;
		m_ViewHeight = height;
	}

	void CameraSystem::SetViewportSize(const float& width, const float& height)
	{
		m_ViewWidth = static_cast<uint32_t>(width);
		m_ViewHeight = static_cast<uint32_t>(height);
	}

}