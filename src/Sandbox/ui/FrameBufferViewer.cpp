#include "ui/FramebufferViewer.h"

FrameBufferViewerElement::FrameBufferViewerElement()
	: m_FrameBuffer(nullptr), m_ContentRegionAvailable(0.0f, 0.0f)
{
}

FrameBufferViewerElement::~FrameBufferViewerElement()
{
	m_FrameBuffer.reset();
}

void FrameBufferViewerElement::OnUpdate(const Ares::Timestep& ts)
{
	if (m_ContentRegionAvailable.x && m_ContentRegionAvailable.y)
	{
		uint32_t newWidth = static_cast<uint32_t>(m_ContentRegionAvailable.x);
		uint32_t newHeight = static_cast<uint32_t>(m_ContentRegionAvailable.y);

		if (m_FrameBuffer == nullptr)
			m_FrameBuffer = Ares::FrameBuffer::Create(newWidth, newHeight);
		else if (m_FrameBuffer->GetWidth() != newWidth || m_FrameBuffer->GetHeight() != newHeight)
		{
			m_FrameBuffer->Resize(newWidth, newHeight);
		}
	}
}

void FrameBufferViewerElement::Draw()
{
	ImGui::Begin("FramebufferViewer");
	m_ContentRegionAvailable = ImGui::GetContentRegionAvail();

	if (m_FrameBuffer)
	{
		float frameBufferWidth = static_cast<float>(m_FrameBuffer->GetWidth());
		float frameBufferHeight = static_cast<float>(m_FrameBuffer->GetHeight());

		// UV coordinates flipping the texture vertically
		ImGui::Image(
			static_cast<ImTextureID>(m_FrameBuffer->GetTextureHandle()),
			m_ContentRegionAvailable,
			{ 0.0f, 1.0f },
			{ 1.0f, 0.0f }
		);
	}

	ImGui::End();
}