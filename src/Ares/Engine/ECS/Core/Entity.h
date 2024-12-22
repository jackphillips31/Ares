#pragma once

namespace Ares::ECS {

	class EntityManager;

	class Entity
	{
	public:
		Entity() = default;
		Entity(uint32_t id, EntityManager* manager);

		// Entity attributes
		void SetName(const std::string& name);
		const std::string GetName();
		const uint32_t& GetID() const;
		const bool IsValid() const;

		// Component related methods
		template <typename ECSComponent, typename... Args>
		ECSComponent* AddComponent(Args&&... args);
		template <typename ECSComponent>
		void RemoveComponent();
		template <typename ECSComponent>
		ECSComponent* GetComponent();

	private:
		uint32_t m_ID = 0;
		EntityManager* m_Manager = nullptr;
	};

}

#include "Engine/ECS/Core/EntityManager.h"

namespace Ares::ECS {

	template <typename ECSComponent, typename... Args>
	inline ECSComponent* Entity::AddComponent(Args&&... args)
	{
		if (m_Manager)
		{
			return m_Manager->AddComponent<ECSComponent>(*this, std::forward<Args>(args)...);
		}
		return nullptr;
	}
	template <typename ECSComponent>
	inline void Entity::RemoveComponent()
	{
		if (m_Manager)
		{
			m_Manager->RemoveComponent<ECSComponent>(*this);
		}
	}
	template <typename ECSComponent>
	inline ECSComponent* Entity::GetComponent()
	{
		if (m_Manager)
		{
			return m_Manager->GetComponent<ECSComponent>(*this);
		}
		return nullptr;
	}

}

