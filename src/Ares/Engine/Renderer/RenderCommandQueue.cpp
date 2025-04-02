#include <arespch.h>
#include "Engine/Renderer/RenderCommandQueue.h"

#include "Engine/Renderer/CommandQueue/Commands.h"

namespace Ares::Internal {

	Scope<RenderCommandQueue> RenderCommandQueue::Create(RendererAPI* api)
	{
		return CreateScope<RenderCommandQueue>(api);
	}

	RenderCommandQueue::RenderCommandQueue(Internal::RendererAPI* api)
		: m_API(api)
	{
	}

	RenderCommandQueue::~RenderCommandQueue()
	{
		while (!m_ReadQueue.empty())
			m_ReadQueue.pop();

		while (!m_WriteQueue.empty())
			m_WriteQueue.pop();
	}

	void RenderCommandQueue::ExecuteCommands()
	{
		SwapQueues();

		std::unique_lock readLock(m_ReadMutex);
		while (!m_ReadQueue.empty())
		{
			Scope<Internal::RendererCommand> command = eastl::move(m_ReadQueue.front());
			m_ReadQueue.pop();
			command->Execute(m_API);
		}
	}

	void RenderCommandQueue::SwapQueues()
	{
		std::unique_lock lock1(m_ReadMutex, std::defer_lock);
		std::unique_lock lock2(m_WriteMutex, std::defer_lock);

		std::lock(m_ReadMutex, m_WriteMutex);
		eastl::swap(m_ReadQueue, m_WriteQueue);
		m_ReadMutex.unlock();
		m_WriteMutex.unlock();
	}

}