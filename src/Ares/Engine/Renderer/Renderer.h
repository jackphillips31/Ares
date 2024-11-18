#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Ares {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}