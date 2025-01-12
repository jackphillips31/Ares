#pragma once
#include <glm/vec2.hpp>

#include "Engine/ECS/Core/System.h"

namespace Ares::ECS {
	
	class Entity;

	namespace Systems {

		class CameraSystem : public System
		{
		public:
			CameraSystem();

			void OnUpdate(const Scene& scene, const Timestep& timestep) override;

			// Getters - Using shared locks for read operations
			const uint32_t GetActiveCameraEntityId();
			const glm::vec2 GetViewportSize();

			// Setters - Using unique locks for write operations
			void SetActiveCamera(const uint32_t& entityId);
			void SetActiveCamera(const Entity& entity);
			void SetViewportSize(const glm::vec2& viewSize);

		private:
			mutable std::shared_mutex m_Mutex;
			glm::vec2 m_ViewportSize;
			uint32_t m_ActiveCameraEntityId;
		};
	
	}

}