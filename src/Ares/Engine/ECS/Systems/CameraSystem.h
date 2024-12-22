#pragma once

#include "Engine/ECS/Core/System.h"

namespace Ares::ECS::Systems {

	class CameraSystem : public System
	{
	public:
		void OnUpdate(Scene& scene, Timestep timestep) override;

		void SetViewportSize(const uint32_t& width, const uint32_t& height);
		void SetViewportSize(const float& width, const float& height);

	private:
		uint32_t m_ViewWidth = 1280;
		uint32_t m_ViewHeight = 720;
	};

}