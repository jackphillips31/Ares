#pragma once

namespace Ares {

	class Timestep;
	
	namespace ECS {

		class Scene;

		class System
		{
		public:
			virtual ~System() = default;

			// Called once when the system is registered in the ECS framework
			virtual void OnInit(const Scene& scene) {}

			// Called when the system is being removed or cleaned up
			virtual void OnShutdown(const Scene& scene) {}

			// Called every OnUpdate
			virtual void OnUpdate(const Scene& scene, const Timestep& timestep) = 0;

			// Called every OnRender
			virtual void OnRender(const Scene& scene) {}
		};

	}

}