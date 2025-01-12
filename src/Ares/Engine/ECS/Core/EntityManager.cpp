#include <arespch.h>
#include "Engine/ECS/Core/EntityManager.h"

namespace Ares::ECS {

	Entity EntityManager::CreateEntity()
	{
		std::unique_lock lock(m_IdMutex);
		uint32_t id = m_NextEntityID++;
		return Entity(id, this);
	}

	void EntityManager::DestroyEntity(Entity& entity)
	{
		std::unique_lock lock(m_ComponentMutex);
		m_Components.erase(entity.GetID());
	}

	void EntityManager::SetEntityName(Entity& entity, const std::string& name)
	{
		std::unique_lock lock(m_MapMutex);
		m_EntityNameMap[entity.GetID()] = name;
		m_NameEntityMap[name] = entity.GetID();
	}

	Entity EntityManager::GetEntity(const uint32_t& id)
	{
		std::shared_lock lock(m_IdMutex);
		if (id < m_NextEntityID)
		{
			return Entity(id, this);
		}
		return Entity(0, nullptr);
	}

	Entity EntityManager::GetEntity(const std::string& name)
	{
		{
			std::shared_lock lock(m_MapMutex);
			if (m_NameEntityMap.find(name) != m_NameEntityMap.end())
				return Entity(m_NameEntityMap[name], this);
		}
		Entity result = CreateEntity();
		SetEntityName(result, name);
		return result;
	}

	const std::string EntityManager::GetEntityName(Entity& entity)
	{
		return GetEntityName(entity.GetID());
	}

	const std::string EntityManager::GetEntityName(const uint32_t& entityId)
	{
		std::shared_lock lock(m_MapMutex);
		return m_EntityNameMap[entityId];
	}

	const std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>>& EntityManager::GetEntityComponents()
	{
		std::shared_lock lock(m_ComponentMutex);
		return m_Components;
	}

}