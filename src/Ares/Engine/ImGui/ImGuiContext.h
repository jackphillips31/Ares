#pragma once
#include "Engine/Core/Memory.h"

namespace Ares {

	class ImGuiContext
	{
	public:
		ImGuiContext() = default;
		virtual ~ImGuiContext() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;

		static AppScope<ImGuiContext> Create();
	};

}