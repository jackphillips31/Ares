#pragma once

#include <Ares.h>

class FrameBufferViewerElement : public Ares::ImGuiElement
{
public:
	FrameBufferViewerElement();
	~FrameBufferViewerElement() override;

	void BeginDraw() override;
	void EndDraw() override;

private:
	Ares::Ref<Ares::FrameBuffer> m_FrameBuffer;
};