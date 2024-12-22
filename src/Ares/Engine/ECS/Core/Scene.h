#pragma once

namespace Ares::ECS {

	class Scene;

	class System
	{
	public:
		virtual ~System() = default;

		// Called once when the system is registered in the ECS framework
		virtual void OnInit(Scene& scene) {}

		// Called when the system is being removed or cleaned up
		virtual void OnShutdown(Scene& scene) {}

		// Called every OnUpdate
		virtual void OnUpdate(Scene& scene, Timestep timestep) = 0;

		// Called every OnRender
		virtual void OnRender(Scene& scene) {}
	};

	class Scene
	{
	public:
		Scene() = default;

		// Initialize & shutdown all systems
		void Init();
		void Shutdown();

		// Update & render all relevant systems
		void OnUpdate(Timestep timestep);
		void OnRender();

		// Getter for the entity manager
		EntityManager* GetEntityManager();

		// Getter for systems
		template <typename System>
		System* GetSystem();

		// Register a system with the scene
		template <typename System, typename... Args>
		void RegisterSystem(Args&&... args);

		// Set system update priority
		template <typename... Systems>
		void SetSystemUpdateOrder();

		// Set system render order
		template <typename... Systems>
		void SetSystemRenderOrder();

	private:
		Scope<EntityManager> m_EntityManager = CreateScope<EntityManager>();
		std::unordered_map<uint32_t, Scope<System>> m_Systems;
		std::atomic<uint32_t> m_NextSystemId{ 1 };
		std::unordered_map<std::type_index, uint32_t> m_SystemIdMap;
		std::vector<System*> m_UpdateOrder;
		std::vector<System*> m_RenderOrder;
	};

	template <typename System, typename... Args>
	void Scene::RegisterSystem(Args&&... args)
	{
		uint32_t currentId = m_NextSystemId++;
		m_Systems[currentId] = CreateScope<System>(std::forward<Args>(args)...);
		m_SystemIdMap[typeid(System)] = currentId;
	}

	template <typename System>
	System* Scene::GetSystem()
	{
		if (m_SystemIdMap.find(typeid(System)) != m_SystemIdMap.end())
		{
			return static_cast<System*>(m_Systems[m_SystemIdMap[typeid(System)]].get());
		}
		else
		{
			return nullptr;
		}
	}

	template <typename... Systems>
	void Scene::SetSystemUpdateOrder()
	{
		m_UpdateOrder.clear();

		// Process each system type in the parameter pack
		(void([&]() {
			// Find the system id
			auto it = m_SystemIdMap.find(typeid(Systems));
			if (it != m_SystemIdMap.end())
			{
				// Add the system pointer to the update order
				m_UpdateOrder.push_back(m_Systems[it->second].get());
			}
			else
			{
				AR_CORE_WARN("Scene does not have this system!");
			}
		}()), ...);
	}

	template <typename... Systems>
	void Scene::SetSystemRenderOrder()
	{
		m_RenderOrder.clear();

		// Process each system type in parameter pack
		(void([&]() {
			// Find the system id
			auto it = m_SystemIdMap.find(typeid(Systems));
			if (it != m_SystemIdMap.end())
			{
				// Add the system pointer to the render order
				m_RenderOrder.push_back(m_Systems[it->second].get());
			}
			else
			{
				AR_CORE_WARN("Scene does not have this system!");
			}
		}()), ...);
	}

}