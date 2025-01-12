#include <arespch.h>
#include "Engine/ECS/Core/Entity.h"

namespace Ares::ECS {

	Entity::Entity()
		: m_ID(0), m_Manager(nullptr)
	{
	}

	Entity::Entity(uint32_t id, EntityManager* manager)
		: m_ID(id), m_Manager(manager)
	{
	}

	Entity::Entity(const Entity& other)
		: m_ID(other.m_ID), m_Manager(other.m_Manager)
	{
	}

	Entity& Entity::operator=(const Entity& other)
	{
		m_ID = other.m_ID;
		m_Manager = other.m_Manager;
		return *this;
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
		std::shared_lock<std::shared_mutex> lock(m_Mutex);
		return m_ID;
	}

	const bool Entity::IsValid() const
	{
		std::shared_lock<std::shared_mutex> lock(m_Mutex);
		return m_Manager != nullptr;
	}

}