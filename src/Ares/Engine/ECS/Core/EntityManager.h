#pragma once

namespace Ares::ECS {

	struct Component
	{
		virtual ~Component() = default;
	};

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
		const std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>>& GetEntityMap();

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
		ECSComponent* GetComponent(Entity& entity);
		template <typename ECSComponent>
		ECSComponent* GetComponent(uint32_t entityId);

	private:
		uint32_t m_NextEntityID = 1;
		std::unordered_map<uint32_t, std::unordered_map<std::type_index, Scope<Component>>> m_Components;
		std::unordered_map<std::type_index, std::vector<uint32_t>> m_ComponentMap;
		std::unordered_map<uint32_t, std::string> m_EntityNameMap;
		std::unordered_map<std::string, uint32_t> m_NameEntityMap;
	};

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

	// AddComponent
	template <typename ECSComponent, typename... Args>
	inline ECSComponent* EntityManager::AddComponent(Entity& entity, Args&&... args)
	{
		return AddComponent<ECSComponent>(entity.GetID(), std::forward<Args>(args)...);
	}
	template <typename ECSComponent, typename... Args>
	inline ECSComponent* EntityManager::AddComponent(uint32_t entityId, Args&&... args)
	{
		auto& components = m_Components[entityId];
		components[typeid(ECSComponent)] = CreateScope<ECSComponent>(std::forward<Args>(args)...);
		m_ComponentMap[typeid(ECSComponent)].push_back(entityId);

		return static_cast<ECSComponent*>(components[typeid(ECSComponent)].get());
	}

	// ===========================
	// ------ EntityManager ------
	// ===========================
	// RemoveComponent
	template <typename ECSComponent>
	inline void EntityManager::RemoveComponent(Entity& entity)
	{
		RemoveComponent<ECSComponent>(entity.GetID());
	}
	template <typename ECSComponent>
	inline void EntityManager::RemoveComponent(uint32_t entityId)
	{
		auto& components = m_Components[entityId];
		components.erase(typeid(ECSComponent));
		m_ComponentMap[typeid(ECSComponent)].erase(entityId);
	}

	// GetComponent
	template <typename ECSComponent>
	inline ECSComponent* EntityManager::GetComponent(Entity& entity)
	{
		return GetComponent<ECSComponent>(entity.GetID());
	}
	template <typename ECSComponent>
	inline ECSComponent* EntityManager::GetComponent(uint32_t entityId)
	{
		auto& components = m_Components[entityId];
		auto it = components.find(typeid(ECSComponent));
		if (it != components.end())
		{
			return static_cast<ECSComponent*>(it->second.get());
		}
		return nullptr;
	}

	// ===========================
	// --------- Entity ----------
	// ===========================
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