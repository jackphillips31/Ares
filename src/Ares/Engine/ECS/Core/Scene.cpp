#include <arespch.h>

#include "Engine/Core/Timestep.h"
#include "Engine/ECS/Core/EntityManager.h"

#include "Engine/ECS/Core/Scene.h"

namespace Ares::ECS {

	void Scene::Init()
	{
		for (auto& system : m_Systems)
		{
			system.second->OnInit(*this);
		}
	}

	void Scene::Shutdown()
	{
		for (auto& system : m_Systems)
		{
			system.second->OnShutdown(*this);
		}
	}

	void Scene::OnUpdate(Timestep timestep)
	{
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
			for (auto& entry : m_Systems)
			{
				entry.second->OnUpdate(*this, timestep);
			}
		}
	}

	void Scene::OnRender()
	{
		for (System* system : m_RenderOrder)
		{
			system->OnRender(*this);
		}
	}

	EntityManager* Scene::GetEntityManager()
	{
		return m_EntityManager.get();
	}

}