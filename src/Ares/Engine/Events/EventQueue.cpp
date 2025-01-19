#include <arespch.h>
#include "Engine/Events/EventQueue.h"

#include "Engine/Core/Application.h"
#include "Engine/Events/Event.h"

namespace Ares {

	void EventQueue::Init()
	{
		AR_CORE_INFO("Initializing EventQueue");
		Shutdown();
	}

	void EventQueue::Shutdown()
	{
		// Clear Read Event Queue
		{
			std::lock_guard<std::mutex> lock(s_ReadQueueMutex);
			while (!s_ReadEventQueue.empty()) s_ReadEventQueue.pop();
		}

		// Clear Write Event Queue
		{
			std::lock_guard<std::mutex> lock(s_WriteQueueMutex);
			while (!s_WriteEventQueue.empty()) s_WriteEventQueue.pop();
		}

		// Clear all listeners
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);
			s_Listeners.clear();
			s_ListenerTypeMap.clear();
		}

		// Reset Id counter
		s_NextListenerId = 1;
	}

	void EventQueue::OnUpdate()
	{
		SwapQueues();

		std::lock_guard<std::mutex> lock(s_ReadQueueMutex);
		while (!s_ReadEventQueue.empty())
		{
			Scope<Event> event = std::move(s_ReadEventQueue.front());
			s_ReadEventQueue.pop();
			if (s_Callback != nullptr)
				s_Callback(*event);
			NotifyListeners(*event);
		}
	}

	void EventQueue::RemoveListener(EventListener& listenerId)
	{
		// Check to see if listener is valid
		if (!listenerId)
		{
			AR_CORE_WARN("Event listener has already been removed!");
			return;
		}

		// Remove listener
		{
			size_t listenersRemoved = 0;
			std::lock_guard<std::mutex> lock(s_ListenerMutex);

			// Find EventType for this listener
			auto it = s_ListenerTypeMap.find(listenerId);
			if (it != s_ListenerTypeMap.end())
			{
				// Erase event listener
				auto& listeners = s_Listeners[it->second];
				listenersRemoved += listeners.erase(listenerId);
				s_ListenerTypeMap.erase(it);
			}

			// Check to see if a listener was removed
			if (!listenersRemoved)
			{
				AR_CORE_WARN("Did not find any event listeners with id: {}", listenerId);
			}
			else
				listenerId = 0;
		}
	}

	void EventQueue::SetEventCallback(ApplicationEventCallbackFn&& callback)
	{
		std::lock_guard<std::mutex> lock(s_CallbackMutex);
		s_Callback = std::move(callback);
	}

	void EventQueue::NotifyListeners(Event& e)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		auto& listeners = s_Listeners[e.GetEventType()];

		for (auto& listenerEntry : listeners)
		{
			if (e.Handled)
				break;

			e.Handled = listenerEntry.second(e);
		}
	}

	void EventQueue::SwapQueues()
	{
		std::lock_guard<std::mutex> writeLock(s_WriteQueueMutex);
		std::lock_guard<std::mutex> readLock(s_ReadQueueMutex);

		std::swap(s_WriteEventQueue, s_ReadEventQueue);
	}
}