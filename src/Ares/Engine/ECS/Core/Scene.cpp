#include <arespch.h>
#include "Engine/ECS/Core/Scene.h"

#include "Engine/Core/Timestep.h"
#include "Engine/ECS/Core/EntityManager.h"
#include "Engine/ECS/Core/System.h"

namespace Ares::ECS {

	void Scene::Init()
	{
		std::shared_lock<std::shared_mutex> lock(m_MapMutex);
		for (auto& system : m_Systems)
		{
			system.second->OnInit(*this);
		}
	}

	void Scene::Shutdown()
	{
		std::shared_lock<std::shared_mutex> lock(m_MapMutex);
		for (auto& system : m_Systems)
		{
			system.second->OnShutdown(*this);
		}
	}

	void Scene::OnUpdate(const Timestep& timestep)
	{
		std::shared_lock<std::shared_mutex> lock(m_OrderMutex);
		if (m_UpdateOrder.size() > 0)
		{
			// Update based on update order
			for (System* system : m_UpdateOrder)
			{
				system->OnUpdate(*this, timestep);
			}
		}
		else
		{
			// If no update order, update all systems randomly
			std::shared_lock<std::shared_mutex> lock(m_MapMutex);
			for (auto& entry : m_Systems)
			{
				entry.second->OnUpdate(*this, timestep);
			}
		}
	}

	void Scene::OnRender()
	{
		std::shared_lock<std::shared_mutex> lock(m_OrderMutex);
		for (System* system : m_RenderOrder)
		{
			system->OnRender(*this);
		}
	}

	EntityManager* Scene::GetEntityManager() const
	{
		return m_EntityManager.get();
	}

}