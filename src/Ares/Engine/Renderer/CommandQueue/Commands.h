#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Engine/Containers/String.h"

namespace Ares {

	class FrameBuffer;
	class ShaderProgram;
	class VertexArray;
	class Texture;

	namespace ECS::Components {

		class Material;
		class Mesh;

	}

	namespace ECS::Systems {

		class RenderSystem;
		struct MeshBatch;

	}

	namespace Internal {

		class RendererAPI;

		struct RendererCommand
		{
		public:
			virtual ~RendererCommand() = default;
			virtual void Execute(RendererAPI* api) = 0;
		};

	}

	namespace RenderCommands {

		struct BindFrameBuffer : public Internal::RendererCommand
		{
		public:
			BindFrameBuffer(FrameBuffer* buffer)
				: Buffer(buffer)
			{
			}

			FrameBuffer* Buffer;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct UnbindFrameBuffer : public Internal::RendererCommand
		{
		public:
			UnbindFrameBuffer(FrameBuffer* buffer)
				: Buffer(buffer)
			{
			}
			FrameBuffer* Buffer;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct ResizeFrameBuffer : public Internal::RendererCommand
		{
		public:
			ResizeFrameBuffer(FrameBuffer* buffer, uint32_t width, uint32_t height)
				: Buffer(buffer), Width(width), Height(height)
			{
			}
			FrameBuffer* Buffer;
			uint32_t Width;
			uint32_t Height;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct BindVertexArray : public Internal::RendererCommand
		{
		public:
			BindVertexArray(const Ref<VertexArray>& vertexArray)
				: VertexArray(vertexArray)
			{
			}
			Ref<VertexArray> VertexArray;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct UnbindVertexArray : public Internal::RendererCommand
		{
		public:
			UnbindVertexArray(const Ref<VertexArray>& vertexArray)
				: VertexArray(vertexArray)
			{
			}
			Ref<VertexArray> VertexArray;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct BindShader : public Internal::RendererCommand
		{
		public:
			BindShader(ShaderProgram* shader)
				: Shader(shader)
			{
			}
			ShaderProgram* Shader;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct UnbindShader : public Internal::RendererCommand
		{
		public:
			UnbindShader(ShaderProgram* shader)
				: Shader(shader)
			{
			}
			ShaderProgram* Shader;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct BindTexture : public Internal::RendererCommand
		{
		public:
			BindTexture(Texture* texture, uint32_t slot = 0)
				: TextureData(texture), Slot(slot)
			{
			}
			Texture* TextureData;
			uint32_t Slot;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct UnbindTexture : public Internal::RendererCommand
		{
		public:
			UnbindTexture(Texture* texture)
				: TextureData(texture)
			{
			}
			Texture* TextureData;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct BindMaterial : public Internal::RendererCommand
		{
		public:
			BindMaterial(ECS::Components::Material* mat)
				: Material(mat)
			{
			}
			ECS::Components::Material* Material;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct SetViewport : public Internal::RendererCommand
		{
		public:
			SetViewport(const glm::uvec2& pos, const glm::uvec2& size)
				: Position(pos), Dimensions(size)
			{
			}
			glm::uvec2 Position;
			glm::uvec2 Dimensions;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct SetClearColor : public Internal::RendererCommand
		{
		public:
			SetClearColor(float r, float g, float b, float a)
				: Color({ r, g, b, a })
			{
			}
			glm::vec4 Color;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct SetFaceCulling : public Internal::RendererCommand
		{
		public:
			bool IsSet;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct Clear : public Internal::RendererCommand
		{
		public:
			void Execute(Internal::RendererAPI* api) override;
		};

		struct Finish : public Internal::RendererCommand
		{
		public:
			void Execute(Internal::RendererAPI* api) override;
		};

		struct Flush : public Internal::RendererCommand
		{
		public:
			void Execute(Internal::RendererAPI* api) override;
		};

		struct DrawIndexed : public Internal::RendererCommand
		{
		public:
			Ref<VertexArray> VertexArray;
			uint32_t IndexCount;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct DrawInstanced : public Internal::RendererCommand
		{
		public:
			DrawInstanced(const Ref<VertexArray>& vert, const uint32_t count)
				: VertexArray(vert), InstanceCount(count)
			{
			}
			Ref<VertexArray> VertexArray;
			uint32_t InstanceCount;

			void Execute(Internal::RendererAPI* api) override;
		};

		template <typename PropertyType>
		struct SetMaterialUniform : public Internal::RendererCommand
		{
		public:
			SetMaterialUniform(ShaderProgram* shader, const String& name, const PropertyType& value)
				: Shader(shader), Name(name), ValueData(value)
			{
			}
			ShaderProgram* Shader;
			String Name;
			PropertyType ValueData;

			void Execute(Internal::RendererAPI* api) override;
		};

		struct CreateBatchVAO : public Internal::RendererCommand
		{
		public:
			CreateBatchVAO(ECS::Systems::MeshBatch* batch, ECS::Components::Mesh* mesh)
				: BatchData(batch), MeshData(mesh)
			{
			}
			ECS::Systems::MeshBatch* BatchData = nullptr;
			ECS::Components::Mesh* MeshData = nullptr;

			void Execute(Internal::RendererAPI* api) override;
		};

	}
}