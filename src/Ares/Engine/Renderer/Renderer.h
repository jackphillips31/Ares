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

	}

	namespace Systems {

		class Renderer : public Internal::System
		{
		public:
			~Renderer();

			void OnClientResize(const uint32_t width, const uint32_t height);
			Internal::RenderCommand* RenderCommand();

			static Scope<Renderer> Create();

			static RenderAPI GetAPI();

		private:
			Renderer();

			template <typename ObjectType, typename... Args>
			friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

		private:
			Scope<Internal::RendererAPI> m_RendererAPI;
			Scope<Internal::RenderCommand> m_RenderCommand;
		};

	}

}