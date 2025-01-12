#pragma once

namespace Ares {

	class Timestep;
	
	namespace ECS {

		class EntityManager;
		class System;

		class Scene
		{
		public:
			Scene() = default;

			// Initialize & shutdown all systems
			void Init();
			void Shutdown();

			// Update & render all relevant systems
			void OnUpdate(const Timestep& timestep);
			void OnRender();

			// Getter for the entity manager
			EntityManager* GetEntityManager() const;

			// Getter for systems
			template <typename System>
			System* GetSystem() const;

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
			std::atomic<uint32_t> m_NextSystemId{ 1 };
			std::unordered_map<uint32_t, Scope<System>> m_Systems;
			std::unordered_map<std::type_index, uint32_t> m_SystemIdMap;
			std::vector<System*> m_UpdateOrder;
			std::vector<System*> m_RenderOrder;
			mutable std::shared_mutex m_MapMutex;
			mutable std::shared_mutex m_OrderMutex;
			mutable std::shared_mutex m_IdMutex;
		};

		template <typename System, typename... Args>
		void Scene::RegisterSystem(Args&&... args)
		{
			uint32_t currentId = 0;
			{
				std::unique_lock<std::shared_mutex> lock(m_IdMutex);
				currentId = m_NextSystemId++;
			}
			std::unique_lock<std::shared_mutex> lock(m_MapMutex);
			m_Systems[currentId] = CreateScope<System>(std::forward<Args>(args)...);
			m_SystemIdMap[typeid(System)] = currentId;
		}

		template <typename System>
		System* Scene::GetSystem() const
		{
			std::shared_lock<std::shared_mutex> lock(m_MapMutex);
			auto itMap = m_SystemIdMap.find(typeid(System));
			if (itMap != m_SystemIdMap.end())
			{
				auto itSys = m_Systems.find(itMap->second);
				if (itSys != m_Systems.end())
					return static_cast<System*>(itSys->second.get());

				return nullptr;
			}
			else
			{
				return nullptr;
			}
		}

		template <typename... Systems>
		void Scene::SetSystemUpdateOrder()
		{
			std::unique_lock<std::shared_mutex> lock1(m_OrderMutex);
			std::shared_lock<std::shared_mutex> lock2(m_MapMutex);
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

}