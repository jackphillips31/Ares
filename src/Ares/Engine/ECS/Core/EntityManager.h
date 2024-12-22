#pragma once
#ifndef AR_ECS_ENTITY_MANAGER_H
#define AR_ECS_ENTITY_MANAGER_H

#include <memory>
#include "Engine/ECS/Core/Component.h"
#include "Engine/ECS/Core/Entity.h"

namespace Ares::ECS {

	class EntityManager
	{
	public:
		EntityManager() = default;

		// Create a new Entity
		Entity CreateEntity()
		{
			uint32_t id = m_NextEntityID++;
			return Entity(id, this);
		}

		// Destroy an entity and its components
		void DestroyEntity(Entity entity)
		{
			m_Components.erase(entity.GetID());
		}

		// Add a component to an entity
		template <typename ECSComponent, typename... Args>
		void AddComponent(Entity entity, Args&&... args);

		// Remove a component from an entity
		template <typename ECSComponent>
		void RemoveComponent(Entity entity);

		// Get a component from an entity
		template <typename ECSComponent>
		ECSComponent* GetComponent(Entity entity);

	private:
		uint32_t m_NextEntityID = 1;
		std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>> m_Components;
	};

	template <typename ECSComponent, typename... Args>
	void EntityManager::AddComponent(Entity entity, Args&&... args)
	{
		auto& components = m_Components[entity.GetID()];
		components[std::type_index(typeid(ECSComponent))] = CreateScope<ECSComponent>(std::forward<Args>(args)...);
	}

	template <typename ECSComponent>
	void EntityManager::RemoveComponent(Entity entity)
	{
		auto& components = m_Components[entity.GetID()];
		components.erase(std::type_index(typeid(ECSComponent)));
	}

	template <typename ECSComponent>
	ECSComponent* EntityManager::GetComponent(Entity entity)
	{
		auto& components = m_Components[entity.GetID()];
		auto it = components.find(std::type_index(typeid(ECSComponent)));
		if (it != components.end())
		{
			return static_cast<ECSComponent*>(it->second.get());
		}
		return nullptr;
	}

}

#endif // AR_ECS_ENTITY_MANAGER_H