#include <arespch.h>
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Renderer/RendererAPI.h"

namespace Ares::Internal {

	Scope<RenderCommand> RenderCommand::Create(RendererAPI* rendererAPI)
	{
		return CreateScope<RenderCommand>(rendererAPI);
	}

	RenderCommand::RenderCommand(RendererAPI* rendererAPI)
		: m_RendererAPI(rendererAPI)
	{
	}

	RenderCommand::~RenderCommand()
	{
		m_RendererAPI = nullptr;
	}

	void RenderCommand::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
	{
		m_RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		m_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::SetFaceCulling(const bool set)
	{
		m_RendererAPI->SetFaceCulling(set);
	}

	void RenderCommand::Clear()
	{
		m_RendererAPI->Clear();
	}

	void RenderCommand::Finish()
	{
		m_RendererAPI->Finish();
	}

	void RenderCommand::Flush()
	{
		m_RendererAPI->Flush();
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount)
	{
		m_RendererAPI->DrawIndexed(vertexArray, indexCount);
	}

	void RenderCommand::DrawInstanced(const Ref<VertexArray>& vertexArray, const uint32_t instanceCount)
	{
		m_RendererAPI->DrawInstanced(vertexArray, instanceCount);
	}

}