#pragma once
#include <Ares.h>
#include <imgui.h>

class FrameBufferViewerElement : public Ares::ImGuiElement
{
public:
	FrameBufferViewerElement();
	~FrameBufferViewerElement() override;

	void Draw() override;
	void OnUpdate(const Ares::Timestep& ts) override;

	inline ImVec2 GetContentRegionAvail() { return m_ContentRegionAvailable; }
	inline Ares::FrameBuffer* GetFrameBuffer() { return m_FrameBuffer.get(); }

private:
	Ares::Scope<Ares::FrameBuffer> m_FrameBuffer;
	ImVec2 m_ContentRegionAvailable;
};