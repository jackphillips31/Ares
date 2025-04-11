#pragma once
#include <glm/mat4x4.hpp>

#include "Engine/Containers/Atomic.h"
#include "Engine/ECS/Core/System.h"

namespace Ares {

	class VertexArray;
	class VertexBuffer;
	class UniformBuffer;

	namespace Systems {

		class Renderer;

	}

	namespace ECS {

		namespace Components {

			class Mesh;
			class Material;
			class MaterialProperties;
			class Transform;

		}
		
		namespace Systems {

			struct BatchData
			{
				Ref<VertexArray> VAO = nullptr;
				Components::Material* MaterialComponent = nullptr;
				Scope<VertexBuffer> TransformBuffer = nullptr;
				Scope<VertexBuffer> PropertiesBuffer = nullptr;
				Scope<UniformBuffer> UniformBuffer = nullptr;
				Vector<glm::mat4> Transforms;
				Vector<Components::MaterialProperties> Properties;
				Vector<uint8_t> TempPropsBuffer;
				uint32_t InstanceCount = 0;
			};

			class RenderSystem : public System
			{
			public:
				RenderSystem(Ares::Systems::Renderer* renderSys);

				void OnInit(const Scene& scene);
				void OnShutdown(const Scene& scene);

				void OnUpdate(const Scene& scene, const Timestep& timestep) override;
				void OnRender(const Scene& scene) override;

			private:
				void UpdateInstanceBuffers(const Scene& scene);
				void RenderDynamic(const Scene& scene);
				void SubmitDynamic(
					Components::Mesh* mesh,
					Components::Material* material,
					Components::Transform* transform
				);
				const size_t GenerateBatchKey(
					Components::Mesh* mesh,
					Components::Material* material
				);

			private:
				Ares::Systems::Renderer* m_Renderer;
				Atomic<bool> m_StagingUpdated;
				HashMap<size_t, BatchData> m_StagingBatches;
				HashMap<size_t, BatchData> m_RenderBatches;
				std::shared_mutex m_StagingMutex;
				std::shared_mutex m_RenderMutex;
			};

		}

	}

}