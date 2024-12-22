#pragma once

namespace Ares::ECS {

	namespace Systems {

		class CameraSystem : public System
		{
		public:
			void OnUpdate(Scene& scene, Timestep timestep) override;

		};
	}

}