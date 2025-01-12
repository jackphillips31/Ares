#pragma once
#include "Engine/Renderer/RendererAPI.h"

namespace Ares {

	class Application;

	class Renderer
	{
	private:
		friend class Application;
		static void Init();
		static void Shutdown();

	public:
		static void OnClientResize(const uint32_t width, const uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}