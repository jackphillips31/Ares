#pragma once

#include <Ares.h>

class FrameBufferViewerElement : public Ares::ImGuiElement
{
public:
	FrameBufferViewerElement(const ImVec2& frameBufferSize = ImVec2(6000.0f, 6000.0f));
	~FrameBufferViewerElement() override;

	void Draw() override;

	inline ImVec2 GetContentRegionAvail() { return m_ContentRegionAvailable; }
	inline Ares::Ref<Ares::FrameBuffer> GetFrameBuffer() { return m_FrameBuffer; }

private:
	Ares::Ref<Ares::FrameBuffer> m_FrameBuffer;
	ImVec2 m_FrameBufferSize;
	ImVec2 m_ContentRegionAvailable;
};