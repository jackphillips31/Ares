#include <arespch.h>
#include "Engine/ECS/Systems/RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/Application.h"
#include "Engine/Core/MainThreadQueue.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Data/RawData.h"
#include "Engine/ECS/Components/AllComponents.h"
#include "Engine/ECS/Core/EntityManager.h"
#include "Engine/ECS/Core/Scene.h"
#include "Engine/ECS/Systems/CameraSystem.h"
#include "Engine/ECS/Systems/LightSystem.h"
#include "Engine/Renderer/CommandQueue/Commands.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommandQueue.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "Engine/Utility/Hash.h"

const uint32_t g_defaultWhiteTexture = 0xffffffff;

namespace Ares::ECS::Systems {

	RenderSystem::RenderSystem(Ares::Systems::Renderer* renderSys)
		: m_Renderer(renderSys), m_StagingUpdated(false)
	{

	}

	void RenderSystem::OnInit(const Scene& scene)
	{

	}

	void RenderSystem::OnShutdown(const Scene& scene)
	{

	}

	void RenderSystem::OnUpdate(const Scene& scene, const Timestep& timestep)
	{
		if (!m_StagingUpdated.load())
		{
			EntityManager* entityManager = scene.GetEntityManager();

			for (auto& [key, batch] : m_StagingBatches)
			{
				batch.Transforms.clear();
				batch.Properties.clear();
				batch.InstanceCount = 0;
			}

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

			m_StagingUpdated = true;
		}
	}

	void RenderSystem::OnRender(const Scene& scene)
	{
		if (m_StagingUpdated.load())
		{
			{
				std::unique_lock lock1(m_StagingMutex, std::defer_lock);
				std::unique_lock lock2(m_RenderMutex, std::defer_lock);
				std::lock(lock1, lock2);
				eastl::swap(m_StagingBatches, m_RenderBatches);
			}
			m_StagingUpdated = false;
			UpdateInstanceBuffers(scene);
		}
		RenderDynamic(scene);
	}

	void RenderSystem::UpdateInstanceBuffers(const Scene& scene)
	{
		for (auto& [key, batch] : m_RenderBatches)
		{
			if (batch.VAO)
			{
				Systems::LightSystem* lights = scene.GetSystem<Systems::LightSystem>();
				RawData lightBuffer = lights->GetLightBuffer();

				Vector<uint8_t>& propsBuffer = batch.TempPropsBuffer;
				propsBuffer.resize(batch.Properties.size() * 44);

				size_t offset = 0;
				for (Components::MaterialProperties& props : batch.Properties)
				{
					std::memcpy(propsBuffer.data() + offset, props.GetBuffer(), props.GetSize());
					offset += props.GetSize();
				}

				if (!batch.TransformBuffer)
				{
					batch.TransformBuffer = VertexBuffer::Create(
						{ batch.Transforms.data(), batch.Transforms.size() * sizeof(glm::mat4) },
						BufferUsage::Dynamic
					);

					BufferLayout transformLayout({ { VertexDataType::Transform, true } });
					batch.TransformBuffer->SetBufferLayout(transformLayout);
					batch.VAO->AddVertexBuffer(batch.TransformBuffer.get());
				}
				else if (batch.Transforms.size() * sizeof(glm::mat4) > batch.TransformBuffer->GetSize())
				{
					batch.TransformBuffer->SetData({ batch.Transforms.data(), batch.Transforms.size() * sizeof(glm::mat4) });
				}
				else
				{
					batch.TransformBuffer->SetSubData({ batch.Transforms.data(), batch.Transforms.size() * sizeof(glm::mat4) });
				}

				if (!batch.PropertiesBuffer)
				{
					batch.PropertiesBuffer = VertexBuffer::Create({ propsBuffer.data(), propsBuffer.size() }, BufferUsage::Dynamic);
					BufferLayout propsLayout({
						{ VertexDataType::ColorRGB, true },
						{ VertexDataType::Alpha, true, true },
						{ VertexDataType::Roughness, true, true },
						{ VertexDataType::Metallic, true, true },
						{ VertexDataType::Reflectivity, true, true },
						{ VertexDataType::EmissiveColor, true },
						{ VertexDataType::EmissiveIntensity, true, true }
					});
					batch.PropertiesBuffer->SetBufferLayout(propsLayout);
					batch.VAO->AddVertexBuffer(batch.PropertiesBuffer.get());
				}
				else if (propsBuffer.size() > batch.PropertiesBuffer->GetSize())
				{
					batch.PropertiesBuffer->SetData({ propsBuffer.data(), propsBuffer.size() });
				}
				else
				{
					batch.PropertiesBuffer->SetSubData({ propsBuffer.data(), propsBuffer.size() });
				}

				if (!batch.UniformBuffer)
				{
					batch.UniformBuffer = UniformBuffer::Create(lights->GetLightBuffer(), 0, BufferUsage::Dynamic);
				}
				else if (lightBuffer.Size > batch.UniformBuffer->GetSize())
				{
					batch.UniformBuffer->SetData(lightBuffer);
				}
				else
				{
					batch.UniformBuffer->SetSubData(lightBuffer);
				}
			}
		}
	}

	void RenderSystem::RenderDynamic(const Scene& scene)
	{
		Internal::RenderCommandQueue* commandQueue = m_Renderer->RenderCommandQueue();
		Systems::CameraSystem* cameraSystem = scene.GetSystem<Systems::CameraSystem>();
		Components::Camera* activeCamera = scene.GetEntityManager()->GetComponent<Components::Camera>(cameraSystem->GetActiveCameraEntityId());
		const glm::vec2 viewportSize = cameraSystem->GetViewportSize();

		if (m_Renderer)
		{
			commandQueue->SubmitCommand<RenderCommands::SetClearColor>(0.0f, 0.0f, 0.0f, 1.0f);
			commandQueue->SubmitCommand<RenderCommands::Clear>();
			commandQueue->SubmitCommand<RenderCommands::SetViewport>(
				glm::uvec2(0, 0),
				glm::uvec2(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y))
			);
		}

		for (auto& [key, batch] : m_RenderBatches)
		{
			if (batch.VAO == nullptr || batch.MaterialComponent == nullptr || activeCamera == nullptr || m_Renderer == nullptr)
				continue;

			batch.MaterialComponent->SetUniformProperty("u_ViewProjection", activeCamera->GetViewProjectionMatrix());
			batch.MaterialComponent->SetUniformProperty("u_CameraPosition", activeCamera->GetPosition());
			batch.MaterialComponent->Bind();

			commandQueue->SubmitCommand<RenderCommands::DrawInstanced>(batch.VAO, batch.InstanceCount);
		}
	}

	void RenderSystem::SubmitDynamic(
		Components::Mesh* mesh,
		Components::Material* material,
		Components::Transform* transform
	)
	{
		Internal::RenderCommandQueue* commandQueue = m_Renderer->RenderCommandQueue();
		if (mesh->IsLoaded() && material->IsLoaded())
		{
			const size_t batchKey = GenerateBatchKey(mesh, material);
			BatchData* batch = nullptr;

			auto batchIt = m_StagingBatches.find(batchKey);
			if (batchIt != m_StagingBatches.end())
			{
				batch = &batchIt->second;
			}
			else
			{
				batch = &m_StagingBatches[batchKey];
				batch->Transforms.reserve(100);
				batch->Properties.reserve(100);
			}

			if (!batch->VAO)
			{
				commandQueue->SubmitCommand<RenderCommands::CreateBatchVAO>(batch, mesh);
			}
			else
			{
				if (!batch->MaterialComponent)
				{
					batch->MaterialComponent = material;
				}
				batch->Transforms.push_back(transform->GetTransformationMatrix());
				batch->Properties.push_back(material->GetProperties());
				batch->InstanceCount++;
			}
		}
		else if (mesh != nullptr && material != nullptr && mesh->IsValid() && material->IsValid())
		{
			const size_t batchKey = GenerateBatchKey(mesh, material);
			m_StagingBatches.erase(batchKey);
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