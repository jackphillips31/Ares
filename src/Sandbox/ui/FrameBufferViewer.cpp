#include <imgui.h>

#include "ui/FramebufferViewer.h"

FrameBufferViewerElement::FrameBufferViewerElement()
{
	m_FrameBuffer = Ares::FrameBuffer::Create(640, 480);
}

FrameBufferViewerElement::~FrameBufferViewerElement()
{
	m_FrameBuffer.reset();
}

void FrameBufferViewerElement::BeginDraw()
{
	ImGui::Begin("Framebuffer Viewer");

	m_FrameBuffer->Bind();
}

void FrameBufferViewerElement::EndDraw()
{
	m_FrameBuffer->Unbind();

	ImVec2 availSize = ImGui::GetContentRegionAvail();
	ImGui::Image(
		static_cast<ImTextureID>(m_FrameBuffer->GetColorAttachmentHandle()),
		availSize
	);
	ImGui::End();
}