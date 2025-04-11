#include <arespch.h>
#include "Engine/Renderer/CommandQueue/Commands.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/detail/qualifier.hpp>

#include "Engine/ECS/Components/Material.h"
#include "Engine/ECS/Components/Mesh.h"
#include "Engine/ECS/Systems/RenderSystem.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Assets/Texture.h"
#include "Engine/Utility/Type.h"

template <typename T>
const char* GetTypeName()
{
	return typeid(T).name();
}

#if 0
#define PRINTCOMMAND(type) AR_CORE_INFO("RenderCommandQueue: {}", GetTypeName<type>());
#else
#define PRINTCOMMAND(type)
#endif

namespace Ares::RenderCommands {

	void BindFrameBuffer::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(BindFrameBuffer);
		if (Buffer)
		{
			Buffer->Bind();
		}
		else
		{
			AR_CORE_WARN("Render Command Failed: Attempted to bind null framebuffer!");
		}
	}

	void UnbindFrameBuffer::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(UnbindFrameBuffer);
		if (Buffer)
		{
			Buffer->Unbind();
		}
		else
		{
			AR_CORE_WARN("Render Command Failed: Attempted to unbind null framebuffer!");
		}
	}

	void ResizeFrameBuffer::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(ResizeFrameBuffer);
		if (Buffer)
		{
			Buffer->Resize(Width, Height);
		}
		else
		{
			AR_CORE_WARN("Render Command Failed: Attempted to resize null framebuffer!");
		}
	}

	void BindVertexArray::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(BindVertexArray);
		VertexArray->Bind();
	}

	void UnbindVertexArray::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(UnbindVertexArray);
		VertexArray->Unbind();
	}

	void BindShader::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(BindShader);
		Shader->Bind();
	}

	void UnbindShader::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(UnbindShader);
		Shader->Unbind();
	}

	void BindTexture::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(BindTexture);
		TextureData->Bind(Slot);
	}

	void UnbindTexture::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(UnbindTexture);
		TextureData->Unbind();
	}

	void BindMaterial::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(BindMaterial);
		Material->Bind();
	}
	

	void SetViewport::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(SetViewport);
		api->SetViewport(Position.x, Position.y, Dimensions.x, Dimensions.y);
	}

	void SetClearColor::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(SetClearColor);
		api->SetClearColor(Color);
	}

	void SetFaceCulling::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(SetFaceCulling);
		api->SetFaceCulling(IsSet);
	}

	void Clear::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(Clear);
		api->Clear();
	}

	void Finish::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(Finish);
		api->Finish();
	}

	void Flush::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(Flush);
		api->Flush();
	}

	void DrawIndexed::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(DrawIndexed);
		if (VAO != nullptr)
			api->DrawIndexed(VAO, IndexCount);
	}

	void DrawInstanced::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(DrawInstanced);
		if (VAO != nullptr && InstanceCount > 0)
			api->DrawInstanced(VAO, InstanceCount);
	}

	template <typename PropertyType>
	void SetMaterialUniform<PropertyType>::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(SetMaterialUniform<PropertyType>);
		using T = std::decay_t<decltype(ValueData)>;
		if constexpr (std::is_same_v<T, int32_t>) Shader->SetInt(Name, ValueData);
		else if constexpr (std::is_same_v<T, float>) Shader->SetFloat(Name, ValueData);
		else if constexpr (std::is_same_v<T, glm::vec2>) Shader->SetFloat2(Name, ValueData);
		else if constexpr (std::is_same_v<T, glm::vec3>) Shader->SetFloat3(Name, ValueData);
		else if constexpr (std::is_same_v<T, glm::vec4>) Shader->SetFloat4(Name, ValueData);
		else if constexpr (std::is_same_v<T, glm::mat3>) Shader->SetMat3(Name, ValueData);
		else if constexpr (std::is_same_v<T, glm::mat4>) Shader->SetMat4(Name, ValueData);
		else
		{
			AR_CORE_ASSERT(false, "Unknown Type!");
		}
	}

	template struct SetMaterialUniform<int const&>;
	template struct SetMaterialUniform<int>;
	template struct SetMaterialUniform<float const&>;
	template struct SetMaterialUniform<glm::vec<2, float, glm::defaultp> const&>;
	template struct SetMaterialUniform<glm::vec<3, float, glm::defaultp> const&>;
	template struct SetMaterialUniform<glm::vec<4, float, glm::defaultp> const&>;
	template struct SetMaterialUniform<glm::mat<3, 3, float, glm::defaultp> const&>;
	template struct SetMaterialUniform<glm::mat<4, 4, float, glm::defaultp> const&>;

	void CreateBatchVAO::Execute(Internal::RendererAPI* api)
	{
		PRINTCOMMAND(CreateBatchVAO);
		if (RenderBatchData == nullptr || RenderMeshData == nullptr)
		{
			AR_CORE_ASSERT(false, "Render Commands - CreateBatchVAO: Data not present!");
		}
		RenderBatchData->VAO = VertexArray::Create();
		RenderBatchData->VAO->AddVertexBuffer(RenderMeshData->GetPositionBuffer());
		RenderBatchData->VAO->AddVertexBuffer(RenderMeshData->GetTextureBuffer());
		RenderBatchData->VAO->AddVertexBuffer(RenderMeshData->GetNormalBuffer());
		RenderBatchData->VAO->SetIndexBuffer(RenderMeshData->GetIndexBuffer());
	}

}