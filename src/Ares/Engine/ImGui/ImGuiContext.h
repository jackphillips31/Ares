#pragma once

namespace Ares {

	class ImGuiContext
	{
	public:
		ImGuiContext() = default;
		virtual ~ImGuiContext() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;

		static Scope<ImGuiContext> Create();
	};

}