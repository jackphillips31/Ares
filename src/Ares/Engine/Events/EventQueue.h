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
				std::lock_guard<std::mutex> lock(s_EventQueueMutex);
				while (!s_EventQueue.empty()) s_EventQueue.pop();
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

		template <typename T>
		static void Dispatch(T e)
		{
			std::lock_guard<std::mutex> lock(s_EventQueueMutex);
			s_EventQueue.emplace(CreateScope<T>(e));
		}

		template<typename T>
		static uint32_t AddListener(std::function<bool(T&)> func)
		{
			std::lock_guard<std::mutex> lock(s_ListenerMutex);
			uint32_t currentId = s_NextListenerId++;
			EventListenerEntry currentEntry({ currentId, [func](Event& e) { return func(static_cast<T&>(e)); } });

			s_Listeners[T::GetStaticType()].emplace_back(currentEntry);
			s_ListenerTypeMap[currentId] = T::GetStaticType();
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
			Application& app = Application::Get();
			std::lock_guard<std::mutex> lock(s_EventQueueMutex);
			while (!s_EventQueue.empty())
			{
				Scope<Event> event = std::move(s_EventQueue.front());
				s_EventQueue.pop();
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

	private:
		static inline std::queue<Scope<Event>> s_EventQueue;
		static inline std::mutex s_EventQueueMutex;

		static inline std::atomic<uint32_t> s_NextListenerId{ 1 };
		static inline std::unordered_map<EventType, std::vector<EventListenerEntry>> s_Listeners;
		static inline std::unordered_map<uint32_t, EventType> s_ListenerTypeMap;
		static inline std::mutex s_ListenerMutex;
	};

}