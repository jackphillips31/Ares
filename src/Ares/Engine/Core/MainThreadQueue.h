#pragma once
#include "Engine/Containers/Queue.h"
#include "Engine/Core/System.h"

namespace Ares::Systems {

	class MainThreadQueue : public Internal::System
	{
	public:
		~MainThreadQueue() = default;
		
		void SubmitTask(eastl::function<void()>&& task);
		void OnUpdate(const Timestep& ts) override;

		static Scope<MainThreadQueue> Create();

	private:
		MainThreadQueue();


		template <typename ObjectType, typename... Args>
		friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

	private:
		Queue<eastl::function<void()>> m_ReadQueue;
		Queue<eastl::function<void()>> m_WriteQueue;
		mutable std::shared_mutex m_ReadMutex;
		mutable std::shared_mutex m_WriteMutex;
	};

}