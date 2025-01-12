#pragma once
#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS {

	class Entity;

	class EntityManager
	{
	public:
		EntityManager() = default;

		// Entity related methods
		Entity CreateEntity();
		void DestroyEntity(Entity& entity);
		void SetEntityName(Entity& entity, const std::string& name);

		// Getter methods
		Entity GetEntity(const uint32_t& id);
		Entity GetEntity(const std::string& name);
		const std::string GetEntityName(Entity& entity);
		const std::string GetEntityName(const uint32_t& entityId);
		const std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>>& GetEntityComponents();

		// Add a component to an entity
		template <typename ECSComponent, typename... Args>
		ECSComponent* AddComponent(Entity& entity, Args&&... args);
		template <typename ECSComponent, typename... Args>
		ECSComponent* AddComponent(uint32_t entityId, Args&&... args);

		// Remove a component from an entity
		template <typename ECSComponent>
		void RemoveComponent(Entity& entity);
		template <typename ECSComponent>
		void RemoveComponent(uint32_t entityId);

		// Get a component from an entity
		template <typename ECSComponent>
		ECSComponent* GetComponent(Entity entity);
		template <typename ECSComponent>
		ECSComponent* GetComponent(uint32_t entityId);

	private:
		uint32_t m_NextEntityID = 1;
		std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>> m_Components;
		std::unordered_map<std::type_index, std::vector<uint32_t>> m_ComponentMap;
		std::unordered_map<uint32_t, std::string> m_EntityNameMap;
		std::unordered_map<std::string, uint32_t> m_NameEntityMap;
		std::shared_mutex m_IdMutex;
		std::shared_mutex m_ComponentMutex;
		std::shared_mutex m_MapMutex;
	};

}

#include "Engine/ECS/Core/Entity.h"

namespace Ares::ECS {

	// AddComponent
	template <typename ECSComponent, typename... Args>
	inline ECSComponent* EntityManager::AddComponent(Entity& entity, Args&&... args)
	{
		return AddComponent<ECSComponent>(entity.GetID(), std::forward<Args>(args)...);
	}

	template <typename ECSComponent, typename... Args>
	inline ECSComponent* EntityManager::AddComponent(uint32_t entityId, Args&&... args)
	{
		std::unique_lock<std::shared_mutex> lock(m_ComponentMutex);
		auto& components = m_Components[entityId];
		components[typeid(ECSComponent)] = CreateScope<ECSComponent>(std::forward<Args>(args)...);
		m_ComponentMap[typeid(ECSComponent)].push_back(entityId);
		return static_cast<ECSComponent*>(components[typeid(ECSComponent)].get());
	}

	// RemoveComponent
	template <typename ECSComponent>
	inline void EntityManager::RemoveComponent(Entity& entity)
	{
		RemoveComponent<ECSComponent>(entity.GetID());
	}

	template <typename ECSComponent>
	inline void EntityManager::RemoveComponent(uint32_t entityId)
	{
		std::unique_lock<std::shared_mutex> lock(m_ComponentMutex);
		auto& components = m_Components[entityId];
		components.erase(typeid(ECSComponent));
		m_ComponentMap[typeid(ECSComponent)].erase(entityId);
	}

	// GetComponent
	template <typename ECSComponent>
	inline ECSComponent* EntityManager::GetComponent(Entity entity)
	{
		return GetComponent<ECSComponent>(entity.GetID());
	}

	template <typename ECSComponent>
	inline ECSComponent* EntityManager::GetComponent(uint32_t entityId)
	{
		std::unique_lock<std::shared_mutex> lock(m_ComponentMutex);
		auto& components = m_Components[entityId];
		auto it = components.find(typeid(ECSComponent));
		if (it != components.end())
		{
			return static_cast<ECSComponent*>(it->second.get());
		}
		return nullptr;
	}

}