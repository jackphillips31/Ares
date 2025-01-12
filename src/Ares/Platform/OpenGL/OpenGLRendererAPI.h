#pragma once
#include "Engine/Renderer/RendererAPI.h"

namespace Ares {

	class OpenGLRendererAPI : public RendererAPI
	{
	private:
		void Init() override;
		void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) override;
		void SetClearColor(const glm::vec4& color) override;
		void SetFaceCulling(const bool set) override;
		void Clear() override;
		void Finish() override;
		void Flush() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) override;
		void DrawInstanced(const Ref<VertexArray>& vertexArray, const uint32_t instanceCount) override;
	};

}