#include <arespch.h>

#include "Engine/ECS/Core/Entity.h"

namespace Ares::ECS {

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