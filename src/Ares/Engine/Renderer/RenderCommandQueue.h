#pragma once
#include "Engine/Containers/Atomic.h"
#include "Engine/Containers/Queue.h"

namespace Ares {

	namespace Systems {

		class Renderer;

	}
	
	namespace Internal {

		class RendererAPI;
		struct RendererCommand;

		class RenderCommandQueue
		{
		private:
			RenderCommandQueue(RendererAPI* api);
			~RenderCommandQueue();

			void ExecuteCommands();
			void SwapQueues();

			static Scope<RenderCommandQueue> Create(RendererAPI* api);

		public:
			template <typename Command, typename... Args>
			void SubmitCommand(Args&&... args);

		private:
			template <typename ObjectType, typename... Args>
			friend Scope<ObjectType> Ares::CreateScope(Args&&... args);
			friend class Systems::Renderer;
			RendererAPI* m_API;
			Queue<Scope<Internal::RendererCommand>> m_ReadQueue;
			Queue<Scope<Internal::RendererCommand>> m_WriteQueue;
			std::shared_mutex m_ReadMutex;
			std::shared_mutex m_WriteMutex;
		};

		template <typename CommandType, typename... Args>
		void RenderCommandQueue::SubmitCommand(Args&&... args)
		{
			std::unique_lock lock(m_WriteMutex);
			m_WriteQueue.push(CreateScope<CommandType>(std::forward<Args>(args)...));
		}

	}

}