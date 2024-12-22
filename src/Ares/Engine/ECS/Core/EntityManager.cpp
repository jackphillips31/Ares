#include <arespch.h>

#include "Engine/ECS/Core/EntityManager.h"

namespace Ares::ECS {

	// ===========================
	// ------ EntityManager ------
	// ===========================

	Entity EntityManager::CreateEntity()
	{
		uint32_t id = m_NextEntityID++;
		return Entity(id, this);
	}

	void EntityManager::DestroyEntity(Entity& entity)
	{
		m_Components.erase(entity.GetID());
	}

	void EntityManager::SetEntityName(Entity& entity, const std::string& name)
	{
		m_EntityNameMap[entity.GetID()] = name;
		m_NameEntityMap[name] = entity.GetID();
	}

	Entity EntityManager::GetEntity(const uint32_t& id)
	{
		if (m_Components.find(id) != m_Components.end())
			return Entity(id, this);
		else
			return Entity();
	}

	Entity EntityManager::GetEntity(const std::string& name)
	{
		if (m_NameEntityMap.find(name) != m_NameEntityMap.end())
			return Entity(m_NameEntityMap[name], this);
		else
			return Entity();
	}

	const std::string EntityManager::GetEntityName(Entity& entity)
	{
		return m_EntityNameMap[entity.GetID()];
	}

	const std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>>& EntityManager::GetEntityMap()
	{
		return m_Components;
	}

	// ===========================
	// --------- Entity ----------
	// ===========================

	Entity::Entity(uint32_t id, EntityManager* manager)
		: m_ID(id), m_Manager(manager)
	{
	}

	void Entity::SetName(const std::string& name)
	{
		m_Manager->SetEntityName(*this, name);
	}

	const std::string Entity::GetName()
	{
		return m_Manager->GetEntityName(*this);
	}

	const uint32_t& Entity::GetID() const
	{
		return m_ID;
	}

	const bool Entity::IsValid() const
	{
		return m_Manager != nullptr;
	}

}