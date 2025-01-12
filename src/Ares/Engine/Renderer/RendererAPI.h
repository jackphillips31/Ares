#pragma once
#include <glm/vec4.hpp>

namespace Ares {

	class Renderer;
	class RenderCommand;
	class VertexArray;

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
		};

	private:
		friend class Renderer;
		friend class RenderCommand;
		virtual void Init() = 0;
		virtual void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetFaceCulling(const bool set) = 0;
		virtual void Clear() = 0;
		virtual void Finish() = 0;
		virtual void Flush() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) = 0;
		virtual void DrawInstanced(const Ref<VertexArray>& vertexArray, const uint32_t instanceCount) = 0;

		inline static API GetAPI() { return s_API; }

		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};
}