#pragma once
#include <glm/vec4.hpp>

namespace Ares {

	class VertexArray;
	enum class RenderAPI : uint8_t;

	namespace Systems {

		class Renderer;

	}

	namespace RenderCommands {

		struct SetViewport;
		struct SetClearColor;
		struct SetFaceCulling;
		struct Clear;
		struct Finish;
		struct Flush;
		struct DrawIndexed;
		struct DrawInstanced;

	}

	namespace Internal {

		class RenderCommand;
		struct RendererCommand;

		class RendererAPI
		{
		private:
			friend class Systems::Renderer;
			friend class Internal::RenderCommand;
			friend struct RenderCommands::SetViewport;
			friend struct RenderCommands::SetClearColor;
			friend struct RenderCommands::SetFaceCulling;
			friend struct RenderCommands::Clear;
			friend struct RenderCommands::Finish;
			friend struct RenderCommands::Flush;
			friend struct RenderCommands::DrawIndexed;
			friend struct RenderCommands::DrawInstanced;
			virtual void Init() = 0;
			virtual void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) = 0;
			virtual void SetClearColor(const glm::vec4& color) = 0;
			virtual void SetFaceCulling(const bool set) = 0;
			virtual void Clear() = 0;
			virtual void Finish() = 0;
			virtual void Flush() = 0;

			virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) = 0;
			virtual void DrawInstanced(const Ref<VertexArray>& vertexArray, const uint32_t instanceCount) = 0;

			inline static RenderAPI GetAPI() { return s_API; }
			inline static void SetAPI(const RenderAPI api) { s_API = api; }

			static Scope<RendererAPI> Create(const RenderAPI api);

		private:
			static RenderAPI s_API;
		};

	}

}