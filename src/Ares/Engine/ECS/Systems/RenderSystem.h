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
				Ref<VertexArray> VAO = nullptr;
				Scope<VertexBuffer> TransformBuffer = nullptr;
				Scope<VertexBuffer> PropertiesBuffer = nullptr;
				Scope<UniformBuffer> UniformBuffer = nullptr;
				Components::Material* material = nullptr;
				Components::Mesh* mesh = nullptr;
				std::vector<glm::mat4> transforms;
				std::vector<Components::MaterialProperties> properties;
				Vector<uint8_t> TempPropsBuffer;
				uint32_t instanceCount = 0;
				Atomic<bool> isDirty = false;
				Atomic<bool> isInitialized = false;
				std::shared_mutex Mutex;
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
				std::unordered_map<size_t, MeshBatch> m_DynamicBatches;
			};

		}

	}

}