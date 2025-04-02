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

			struct MeshBatch
			{
				Ref<VertexArray> vao = nullptr;
				Scope<VertexBuffer> transformBuffer = nullptr;
				Scope<VertexBuffer> propertiesBuffer = nullptr;
				Scope<UniformBuffer> uniformBuffer = nullptr;
				Components::Material* material = nullptr;
				std::vector<glm::mat4> transforms;
				std::vector<Components::MaterialProperties> properties;
				uint32_t instanceCount = 0;
				bool isDirty = false;
				size_t transformBufferSize = 0;
				std::shared_mutex mutex;
				Atomic<bool> submittedForCreation = false;
				Atomic<bool> isInitialized = false;
			};

			class RenderSystem : public System
			{
			public:
				RenderSystem(Ares::Systems::Renderer* renderSys);

				void OnInit(const Scene& scene);
				void OnShutdown(const Scene& scene);

				void OnUpdate(const Scene& scene, const Timestep& timestep) override;
				void OnRender(const Scene& scene);

			private:
				void UpdateInstanceBuffers(const Scene& scene);
				void SubmitDynamic(
					Components::Mesh* mesh,
					Components::Material* material,
					Components::Transform* transform
				);
				void RenderDynamic(const Scene& scene);
				const size_t GenerateBatchKey(
					Components::Mesh* mesh,
					Components::Material* material
				);

			private:
				Ares::Systems::Renderer* m_Renderer;
				std::unordered_map<size_t, MeshBatch> m_DynamicBatches;
			};

		}

	}

}