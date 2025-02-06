#include <arespch.h>
#include "Engine/Events/EventQueue.h"


#include "Engine/Core/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Ares::Systems {

	AppScope<EventQueue> EventQueue::Create()
	{
		return CreateAppScope<EventQueue>();
	}

	EventQueue::EventQueue()
		: m_AppCallback(nullptr), m_NextListenerId(1)
	{
	}

	EventQueue::~EventQueue()
	{
		{
			std::unique_lock lock(m_WriteMutex);
			while (!m_WriteQueue.empty()) m_WriteQueue.pop();
		}
		{
			std::unique_lock lock(m_ReadMutex);
			while (!m_ReadQueue.empty()) m_ReadQueue.pop();
		}
		{
			std::unique_lock lock(m_ListenerMutex);
			m_Listeners.clear();
			m_ListenerIndexMap.clear();
		}
		{
			std::unique_lock lock(m_CallbackMutex);
			m_AppCallback = nullptr;
		}
	}

	void EventQueue::Dispatch(Event& e)
	{
		std::unique_lock lock(m_WriteMutex);
		m_WriteQueue.emplace(e.Clone());
	}

	void EventQueue::OnUpdate(Timestep& ts)
	{
		{
			std::unique_lock lock1(m_WriteMutex);
			std::unique_lock lock2(m_ReadMutex);
			eastl::swap(m_WriteQueue, m_ReadQueue);
		}
		{
			std::unique_lock lock(m_ReadMutex);
			while (!m_ReadQueue.empty())
			{
				AppScope<Event> event = eastl::move(m_ReadQueue.front());
				m_ReadQueue.pop();
				if (m_AppCallback)
					m_AppCallback(*event);

				for (auto& listenerEntry : m_Listeners)
				{
					if (event->Handled)
						break;

					if (listenerEntry.ListenerType == EventType::None || listenerEntry.ListenerType == event->GetEventType())
					{
						event->Handled = listenerEntry.CallbackFn(*event);
					}
				}
			}
		}
	}

	void EventQueue::SetEventCallback(ApplicationCallback&& callback)
	{
		std::unique_lock lock(m_CallbackMutex);
		m_AppCallback = eastl::move(callback);
	}

	EventListener EventQueue::AddGlobalListener(StoredCallbackFn&& callback)
	{
		EventListener currentId = m_NextListenerId++;
		ListenerEntry entry;
		entry.CallbackFn = eastl::move(callback);
		entry.ListenerId = currentId;
		entry.ListenerType = EventType::None;

		std::unique_lock lock(m_ListenerMutex);
		m_ListenerIndexMap[currentId] = m_Listeners.size();
		m_Listeners.emplace_back(eastl::move(entry));

		return currentId;
	}

	void EventQueue::RemoveListener(EventListener& listenerId)
	{
		std::unique_lock lock(m_ListenerMutex);
		auto it = m_ListenerIndexMap.find(listenerId);
		if (it != m_ListenerIndexMap.end())
		{
			m_Listeners.erase(m_Listeners.begin() + it->second);
			listenerId = 0;
		}
	}

}