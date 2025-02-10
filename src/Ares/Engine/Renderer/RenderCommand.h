#pragma once
#include <glm/vec4.hpp>

namespace Ares {

	class VertexArray;

	namespace Systems {

		class Renderer;

	}

	namespace Internal {

		class RendererAPI;

		class RenderCommand
		{
		public:
			~RenderCommand();

			void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height);
			void SetClearColor(const glm::vec4& color);
			void SetFaceCulling(const bool set);
			void Clear();
			void Finish();
			void Flush();
			void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0);
			void DrawInstanced(const Ref<VertexArray>& vertexArray, const uint32_t instanceCount = 1);

			static Scope<RenderCommand> Create(RendererAPI* rendererAPI);

		private:
			template <typename ObjectType, typename... Args>
			friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

			RenderCommand(RendererAPI* rendererAPI);

		private:
			RendererAPI* m_RendererAPI;
		};

	}

}