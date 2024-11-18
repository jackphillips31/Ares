#pragma once

#include <imgui.h>

#include "Engine/Layers/ImGuiLayer.h"

namespace Ares {

	class WinImGuiLayer : public ImGuiLayer
	{
	public:
		WinImGuiLayer();
		~WinImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin() override;
		virtual void End() override;

	private:
		float m_Time = 0.0f;
	};

	void CreateViewportContext(ImGuiViewport* viewport);
	void DestroyViewportContext(ImGuiViewport* viewport);
	void RenderViewport(ImGuiViewport* viewport, void* render_arg);

}