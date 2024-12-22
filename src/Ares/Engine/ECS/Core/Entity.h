#pragma once
#ifndef AR_ECS_ENTITY_H
#define AR_ECS_ENTITY_H

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS {

	class EntityManager;

	class Entity
	{
	public:
		Entity() = default;
		Entity(uint32_t id, EntityManager* manager)
			: m_ID(id), m_Name(""), m_Manager(manager)
		{
		}

		inline void AddName(const std::string& name)
		{
			m_Name = name;
		}

		inline const uint32_t& GetID() const { return m_ID; }
		inline const std::string& GetName() const { return m_Name; }
		inline const bool IsValid() const { return m_Manager != nullptr; }

		template <typename ECSComponent, typename... Args>
		void AddComponent(Args&&... args);

		template <typename ECSComponent>
		void RemoveComponent();

		template <typename ECSComponent>
		ECSComponent* GetComponent();

	private:
		uint32_t m_ID = 0;
		std::string m_Name = "";
		EntityManager* m_Manager = nullptr;
	};

}

#include "Engine/ECS/Core/EntityManager.h"

template <typename ECSComponent, typename... Args>
void Ares::ECS::Entity::AddComponent(Args&&... args)
{
	if (m_Manager)
	{
		m_Manager->AddComponent<ECSComponent>(*this, std::forward<Args>(args)...);
	}
}

template <typename ECSComponent>
void Ares::ECS::Entity::RemoveComponent()
{
	if (m_Manager)
	{
		m_Manager->RemoveComponent<ECSComponent>(*this);
	}
}

template <typename ECSComponent>
ECSComponent* Ares::ECS::Entity::GetComponent()
{
	if (m_Manager)
	{
		return m_Manager->GetComponent<ECSComponent>(*this);
	}
	return nullptr;
}

#endif // AR_ECS_ENTITY_H