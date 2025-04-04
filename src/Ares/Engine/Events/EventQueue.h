/**
 * @file EventQueue.h
 * @brief Defines the EventQueue class for managing event processing.
 * 
 * @details The EventQueue class is responsible for queuing and dispatching events in the application.
 * It supports adding listeners for specific event types, as well as global listeners,
 * and propagates events to all listeners.
 */
#pragma once
#include "Engine/Core/System.h"

namespace Ares {

	class Event;
	class Timestep;
	enum class EventType : uint16_t;

	/**
	 * @typedef EventListener
	 * @brief Listener ID stored as a 32 bit unsigned integer.
	 */
	using EventListener = uint32_t;

	namespace Systems {

		class EventQueue : public Internal::System
		{
		private:
			using ApplicationCallback = Function<void(Event&)>;
			template <typename T>
			using ListenerCallbackFn = Function<bool(T&)>;
			using StoredCallbackFn = Function<bool(Event&)>;
		public:
			~EventQueue();

			void Dispatch(Event& e);

			void OnUpdate(const Timestep& ts) override;

			void SetEventCallback(ApplicationCallback&& callback);

			template <typename T>
			EventListener AddListener(ListenerCallbackFn<T>&& callback);

			EventListener AddGlobalListener(StoredCallbackFn&& callback);

			void RemoveListener(EventListener& listenerId);

			static Scope<EventQueue> Create();

		private:
			template <typename ObjectType, typename... Args>
			friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

			EventQueue();

			struct ListenerEntry
			{
				EventListener ListenerId = 0;
				EventType ListenerType = static_cast<EventType>(0);
				StoredCallbackFn CallbackFn = nullptr;
			};

		private:
			Queue<Scope<Event>> m_WriteQueue;
			Queue<Scope<Event>> m_ReadQueue;
			std::shared_mutex m_WriteMutex;
			std::shared_mutex m_ReadMutex;

			Atomic<uint32_t> m_NextListenerId;
			Vector<ListenerEntry> m_Listeners;
			HashMap<EventListener, size_t> m_ListenerIndexMap;
			std::shared_mutex m_ListenerMutex;

			ApplicationCallback m_AppCallback;
			std::shared_mutex m_CallbackMutex;
		};

		template <typename T>
		EventListener EventQueue::AddListener(ListenerCallbackFn<T>&& callback)
		{
			EventListener currentId = m_NextListenerId++;
			ListenerEntry entry;
			entry.CallbackFn = [func = eastl::move(callback)](Event& e) { return func(static_cast<T&>(e)); };
			entry.ListenerId = currentId;
			entry.ListenerType = T::GetStaticType();

			std::unique_lock lock(m_ListenerMutex);
			m_ListenerIndexMap[currentId] = m_Listeners.size();
			m_Listeners.emplace_back(eastl::move(entry));

			return currentId;
		}

	}

}