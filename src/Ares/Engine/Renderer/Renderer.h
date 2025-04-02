#pragma once
#include "Engine/Core/System.h"

#include "Engine/Renderer/RenderCommand.h"

namespace Ares {

	enum class RenderAPI : uint8_t
	{
		None = 0,
		OpenGL = 1
	};

	namespace Internal {

		class RendererAPI;
		class RenderCommand;
		class RenderCommandQueue;

	}

	namespace Systems {

		class Renderer : public Internal::System
		{
		public:
			~Renderer() override;

			void OnRender() override;
			void OnClientResize(const uint32_t width, const uint32_t height);
			Internal::RenderCommand* RenderCommand();
			Internal::RenderCommandQueue* RenderCommandQueue();

			static Scope<Renderer> Create(const RenderAPI api = RenderAPI::OpenGL);

			static RenderAPI GetAPI();
			static void SetAPI(const RenderAPI api);

		private:
			Renderer(const RenderAPI api);

			template <typename ObjectType, typename... Args>
			friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

		private:
			Scope<Internal::RendererAPI> m_RendererAPI;
			Scope<Internal::RenderCommand> m_RenderCommand;
			Scope<Internal::RenderCommandQueue> m_CommandQueue;
		};

	}

}