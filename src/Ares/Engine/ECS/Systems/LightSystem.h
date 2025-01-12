#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Engine/ECS/Core/System.h"

namespace Ares {

	struct RawData;
	
	namespace ECS::Systems {

		class LightSystem : public System
		{
		public:
			LightSystem();

			void OnUpdate(const Scene& scene, const Timestep& timestep) override;

			RawData GetLightBuffer() const;

		private:
			struct LightBuffer
			{
				int32_t Count;
				glm::vec3 Padding = glm::vec3(0.0f);
				struct LightBufferElement
				{
					glm::vec4 Position;
					glm::vec4 Color;
					glm::vec4 Properties;
				} Lights[32];
			};

		private:
			mutable std::shared_mutex m_Mutex;
			LightBuffer m_Buffer;
		};

	}

}