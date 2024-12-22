#pragma once

#include "Engine/Core/Timestep.h"

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

}

#include "Engine/ECS/Core/Scene.h"