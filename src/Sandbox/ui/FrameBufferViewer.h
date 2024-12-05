#pragma once

#include <Ares.h>

class FrameBufferViewerElement : public Ares::ImGuiElement
{
public:
	FrameBufferViewerElement();
	~FrameBufferViewerElement() override;

	void Draw() override;
	void OnUpdate() override;

	inline ImVec2 GetContentRegionAvail() { return m_ContentRegionAvailable; }
	inline Ares::Ref<Ares::FrameBuffer> GetFrameBuffer() { return m_FrameBuffer; }

private:
	Ares::Ref<Ares::FrameBuffer> m_FrameBuffer;
	ImVec2 m_ContentRegionAvailable;
};