#include <arespch.h>
#include "Engine/ECS/Systems/LightSystem.h"

#include "Engine/Core/Timestep.h"
#include "Engine/Data/RawData.h"
#include "Engine/ECS/Components/AllComponents.h"
#include "Engine/ECS/Core/EntityManager.h"
#include "Engine/ECS/Core/Scene.h"

namespace Ares::ECS::Systems {

	LightSystem::LightSystem()
		: m_Buffer()
	{
	}

	void LightSystem::OnUpdate(const Scene& scene, const Timestep& timestep)
	{
		std::unique_lock lock(m_Mutex);
		m_Buffer.Count = 0;
		EntityManager* entityManager = scene.GetEntityManager();

		// Query entities with Transform and Light components
		for (auto& entry : entityManager->GetEntityComponents())
		{
			const uint32_t entityId = entry.first;
			Components::Transform* transform = entityManager->GetComponent<Components::Transform>(entityId);
			Components::Light* light = entityManager->GetComponent<Components::Light>(entityId);
			if (transform != nullptr && light != nullptr)
			{
				// Entity has transform and light component
				const uint32_t lightIndex = m_Buffer.Count++;
				LightBuffer::LightBufferElement& bufferElement = m_Buffer.Lights[lightIndex];

				float lightType;
				switch (light->GetType())
				{
				case Components::Light::Directional: lightType = 0.0f; break;
				case Components::Light::Point: lightType = 1.0f; break;
				default: lightType = 1.0f;
				}

				bufferElement.Position = glm::vec4(transform->GetPosition(), lightType);
				bufferElement.Color = light->GetColor();
				bufferElement.Properties = light->GetProperties();
			}
		}
	}

	RawData LightSystem::GetLightBuffer() const
	{
		std::unique_lock lock(m_Mutex);
		return RawData(&m_Buffer, sizeof(m_Buffer));
	}

}