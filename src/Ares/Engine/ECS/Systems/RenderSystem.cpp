#include <arespch.h>
#include "Engine/ECS/Systems/RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/Timestep.h"
#include "Engine/Core/Utility.h"
#include "Engine/Data/RawData.h"
#include "Engine/ECS/Components/AllComponents.h"
#include "Engine/ECS/Core/EntityManager.h"
#include "Engine/ECS/Core/Scene.h"
#include "Engine/ECS/Systems/CameraSystem.h"
#include "Engine/ECS/Systems/LightSystem.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/UniformBuffer.h"

const uint32_t g_defaultWhiteTexture = 0xffffffff;

namespace Ares::ECS::Systems {

	void RenderSystem::OnInit(const Scene& scene)
	{

	}

	void RenderSystem::OnShutdown(const Scene& scene)
	{

	}

	void RenderSystem::OnUpdate(const Scene& scene, const Timestep& timestep)
	{
		EntityManager* entityManager = scene.GetEntityManager();

		for (const auto& entity : entityManager->GetEntityComponents())
		{
			const uint32_t entityId = entity.first;
			Components::Mesh* mesh = entityManager->GetComponent<Components::Mesh>(entityId);
			Components::Material* material = entityManager->GetComponent<Components::Material>(entityId);
			Components::Transform* transform = entityManager->GetComponent<Components::Transform>(entityId);
			if (mesh != nullptr && material != nullptr && transform != nullptr)
			{
				SubmitDynamic(mesh, material, transform);
			}
		}
	}

	void RenderSystem::OnRender(const Scene& scene)
	{
		UpdateInstanceBuffers(scene);
		RenderDynamic(scene);
	}

	void RenderSystem::UpdateInstanceBuffers(const Scene& scene)
	{
		for (auto& [key, batch] : m_DynamicBatches)
		{
			if (batch.isDirty)
			{
				if (batch.transformBuffer == nullptr)
				{
					batch.transformBufferSize = batch.transforms.size() * sizeof(glm::mat4);
					batch.transformBuffer = VertexBuffer::Create(
						{ batch.transforms.data(), batch.transformBufferSize },
						BufferUsage::Dynamic
					);

					BufferLayout transformLayout({ { VertexDataType::Transform, true } });
					batch.transformBuffer->SetBufferLayout(transformLayout);
					batch.vao->AddVertexBuffer(batch.transformBuffer.get());
				}
				else
				{
					if (batch.transforms.size() * sizeof(glm::mat4) > batch.transformBufferSize)
					{
						batch.transformBufferSize = batch.transforms.size() * sizeof(glm::mat4);
						batch.transformBuffer->SetData({ batch.transforms.data(), batch.transformBufferSize });
					}
					else
					{
						batch.transformBuffer->SetSubData({ batch.transforms.data(), batch.transforms.size() * sizeof(glm::mat4) });
					}
				}

				if (batch.propertiesBuffer == nullptr)
				{
					std::vector<uint8_t> propsBuffer(batch.properties.size() * 56);
					size_t offset = 0;
					for (Components::MaterialProperties& props : batch.properties)
					{
						std::memcpy(propsBuffer.data() + offset, props.GetBuffer(), props.GetSize());
						offset += props.GetSize();
					}

					batch.propertiesBuffer = VertexBuffer::Create({ propsBuffer.data(), propsBuffer.size() }, BufferUsage::Dynamic);
					BufferLayout propsLayout({
						{ VertexDataType::ColorRGB, true },
						{ VertexDataType::Alpha, true, true },
						{ VertexDataType::Roughness, true, true },
						{ VertexDataType::Metallic, true, true },
						{ VertexDataType::Reflectivity, true, true },
						{ VertexDataType::EmissiveColor, true },
						{ VertexDataType::EmissiveIntensity, true, true }
					});
					batch.propertiesBuffer->SetBufferLayout(propsLayout);
					batch.vao->AddVertexBuffer(batch.propertiesBuffer.get());
				}
				else
				{
					std::vector<uint8_t> propsBuffer(batch.properties.size() * 56);
					size_t offset = 0;
					for (Components::MaterialProperties& props : batch.properties)
					{
						std::memcpy(propsBuffer.data() + offset, props.GetBuffer(), props.GetSize());
						offset += props.GetSize();
					}
					if (propsBuffer.size() > batch.propertiesBuffer->GetSize())
					{
						batch.propertiesBuffer->SetData({ propsBuffer.data(), propsBuffer.size() });
					}
					else
					{
						batch.propertiesBuffer->SetSubData({ propsBuffer.data(), propsBuffer.size() });
					}
				}

				if (batch.uniformBuffer == nullptr)
				{
					Systems::LightSystem* lights = scene.GetSystem<Systems::LightSystem>();
					batch.uniformBuffer = UniformBuffer::Create(lights->GetLightBuffer(), 0, BufferUsage::Dynamic);
				}
				else
				{
					Systems::LightSystem* lights = scene.GetSystem<Systems::LightSystem>();
					RawData lightBuffer = lights->GetLightBuffer();
					if (lightBuffer.Size > batch.uniformBuffer->GetSize())
					{
						batch.uniformBuffer->SetData(lightBuffer);
					}
					else
					{
						batch.uniformBuffer->SetSubData(lightBuffer);
					}
				}

				batch.isDirty = false;
			}
		}
	}

	void RenderSystem::SubmitDynamic(
		Components::Mesh* mesh,
		Components::Material* material,
		Components::Transform* transform
	)
	{
		if (mesh->IsLoaded() && material->IsLoaded())
		{
			MeshBatch& batch = m_DynamicBatches[GenerateBatchKey(mesh, material)];

			if (!batch.isDirty)
				batch.instanceCount = 0;

			if (batch.vao == nullptr)
			{
				// New batch
				batch.vao = VertexArray::Create();
				batch.vao->AddVertexBuffer(mesh->GetPositionBuffer());
				batch.vao->AddVertexBuffer(mesh->GetTextureBuffer());
				batch.vao->AddVertexBuffer(mesh->GetNormalBuffer());
				batch.vao->SetIndexBuffer(mesh->GetIndexBuffer());
			}
			if (batch.material == nullptr)
			{
				batch.material = material;
			}
			batch.transforms.push_back(transform->GetTransformationMatrix());
			batch.properties.push_back(material->GetProperties());

			batch.instanceCount++;
			batch.isDirty = true;
		}
		else if (mesh != nullptr && material != nullptr && mesh->IsValid() && material->IsValid())
		{
			const size_t batchKey = GenerateBatchKey(mesh, material);
			m_DynamicBatches.erase(batchKey);
		}
	}

	void RenderSystem::RenderDynamic(const Scene& scene)
	{
		Systems::CameraSystem* cameraSystem = scene.GetSystem<Systems::CameraSystem>();
		Components::Camera* activeCamera = scene.GetEntityManager()->GetComponent<Components::Camera>(cameraSystem->GetActiveCameraEntityId());
		const glm::vec2 viewportSize = cameraSystem->GetViewportSize();

		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();
		RenderCommand::SetViewport(
			0, 0,
			static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y)
		);

		for (auto& [key, batch] : m_DynamicBatches)
		{
			if (activeCamera != nullptr)
			{
				batch.material->SetUniformProperty("u_ViewProjection", activeCamera->GetViewProjectionMatrix());
				batch.material->SetUniformProperty("u_CameraPosition", activeCamera->GetPosition());
			}

			batch.material->Bind();
			
			RenderCommand::DrawInstanced(batch.vao, batch.instanceCount);
			batch.vao->Unbind();
			batch.transforms.clear();
			batch.properties.clear();
		}
	}

	const size_t RenderSystem::GenerateBatchKey(Components::Mesh* mesh, Components::Material* material)
	{
		size_t result = 66688666;
		CombineHash<Components::Mesh>(result, *mesh);
		CombineHash<Components::Material>(result, *material);
		return result;
	}

}