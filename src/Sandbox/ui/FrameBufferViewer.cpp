#include <imgui.h>

#include "ui/FramebufferViewer.h"

FrameBufferViewerElement::FrameBufferViewerElement(const ImVec2& frameBufferSize)
	: m_FrameBufferSize(frameBufferSize), m_ContentRegionAvailable(0.0f, 0.0f)
{
	m_FrameBuffer = Ares::FrameBuffer::Create(m_FrameBufferSize.x, m_FrameBufferSize.y);
}

FrameBufferViewerElement::~FrameBufferViewerElement()
{
	m_FrameBuffer.reset();
}

void FrameBufferViewerElement::Draw()
{
	ImGui::Begin("FramebufferViewer");
	m_ContentRegionAvailable = ImGui::GetContentRegionAvail();

	ImVec2 uv0 = ImVec2(0.0f, 0.0f);
	ImVec2 uv1 = ImVec2(
		m_ContentRegionAvailable.x / m_FrameBufferSize.x,
		m_ContentRegionAvailable.y / m_FrameBufferSize.y
	);
	ImGui::Image(
		static_cast<ImTextureID>(m_FrameBuffer->GetColorAttachmentHandle()),
		m_ContentRegionAvailable,
		uv0,
		uv1
	);

	ImGui::End();
}