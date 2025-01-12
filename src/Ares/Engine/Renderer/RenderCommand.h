#pragma once
#include "Engine/Renderer/RendererAPI.h"

namespace Ares {

	class Renderer;
	class VertexArray;
	
	class RenderCommand
	{
	private:
		friend class Renderer;
		inline static void Init()
		{
			s_RendererAPI = RendererAPI::Create();
			s_RendererAPI->Init();
		}

		inline static void Shutdown()
		{
			s_RendererAPI.reset();
		}

	public:
		inline static void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetFaceCulling(const bool set)
		{
			s_RendererAPI->SetFaceCulling(set);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void Finish()
		{
			s_RendererAPI->Finish();
		}

		inline static void Flush()
		{
			s_RendererAPI->Flush();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawInstanced(const Ref<VertexArray>& vertexArray, const uint32_t instanceCount = 1)
		{
			s_RendererAPI->DrawInstanced(vertexArray, instanceCount);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}