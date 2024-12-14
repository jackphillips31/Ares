#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"

namespace Ares {

	class EventQueue
	{
	public:
		EventQueue() = delete;

		static void Init()
		{
			AR_CORE_INFO("Initializing EventQueue");
			Shutdown();
		}

		static void Shutdown()
		{
			// Clear Event Queue
			{
				//std::lock_guard<std::mutex> lock(s_EventQueueMutex);
				//while (!s_EventQueue.empty()) s_EventQueue.pop();
			}

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

		template <typename EventTypeTemplate>
		static void Dispatch(EventTypeTemplate e)
		{
			std::lock_guard<std::mutex> lock(s_WriteQueueMutex);
			s_WriteEventQueue.emplace(CreateScope<EventTypeTemplate>(e));
		}

		template<typename EventTypeTemplate>
		static uint32_t AddListener(std::function<bool(EventTypeTemplate&)> func)
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);
			uint32_t currentId = s_NextListenerId++;
			EventListenerEntry currentEntry({ currentId, [func](Event& e) { return func(static_cast<EventTypeTemplate&>(e)); } });

			s_Listeners[EventTypeTemplate::GetStaticType()].emplace_back(currentEntry);
			s_ListenerTypeMap[currentId] = EventTypeTemplate::GetStaticType();
			return currentId;
		}

		static void RemoveListener(uint32_t listenerId)
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);

			// Find EventType for this listener
			auto typeIt = s_ListenerTypeMap.find(listenerId);
			if (typeIt == s_ListenerTypeMap.end())
			{
				AR_CORE_WARN("Event Listener not found!");
				return;
			}

			EventType type = typeIt->second;

			// Remove the listener
			auto& listeners = s_Listeners[type];
			listeners.erase(
				std::remove_if(
					listeners.begin(),
					listeners.end(),
					[listenerId](const EventListenerEntry& entry)
					{
						return entry.Id == listenerId;
					}
				),
				listeners.end()
			);
		}

		static void ProcessEvents()
		{
			SwapQueues();

			Application& app = Application::Get();
			std::lock_guard<std::mutex> lock(s_ReadQueueMutex);
			while (!s_ReadEventQueue.empty())
			{
				Scope<Event> event = std::move(s_ReadEventQueue.front());
				s_ReadEventQueue.pop();
				app.OnEvent(*event);
				NotifyListeners(*event);
			}
		}

	private:
		struct EventListenerEntry
		{
			uint32_t Id;
			std::function<bool(Event&)> Function;
		};

	private:
		static void NotifyListeners(Event& e)
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);
			auto& listeners = s_Listeners[e.GetEventType()];

			for (auto& listenerEntry : listeners)
			{
				if (e.Handled)
					break;

				e.Handled = listenerEntry.Function(e);
			}
		}

		static void SwapQueues()
		{
			std::lock_guard<std::mutex> writeLock(s_WriteQueueMutex);
			std::lock_guard<std::mutex> readLock(s_ReadQueueMutex);

			std::swap(s_WriteEventQueue, s_ReadEventQueue);
		}

	private:
		static inline std::queue<Scope<Event>> s_ReadEventQueue;
		static inline std::queue<Scope<Event>> s_WriteEventQueue;
		static inline std::mutex s_ReadQueueMutex;
		static inline std::mutex s_WriteQueueMutex;

		static inline std::atomic<uint32_t> s_NextListenerId{ 1 };
		static inline std::unordered_map<EventType, std::vector<EventListenerEntry>> s_Listeners;
		static inline std::unordered_map<uint32_t, EventType> s_ListenerTypeMap;
		static inline std::mutex s_ListenerMutex;
	};

}