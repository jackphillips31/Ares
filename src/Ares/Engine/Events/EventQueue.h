/**
 * @file EventQueue.h
 * @brief Defines the EventQueue class for managing event processing.
 * 
 * @details The EventQueue class is responsible for queuing and dispatching events in the application.
 * It supports adding listeners for specific event types and propagating events to all listeners.
 */
#pragma once
#include <EASTL/hash_map.h>
#include <EASTL/hash_set.h>

#include "Engine/Core/System.h"
#include "Engine/Core/Utility.h"
#include "Engine/Events/Event.h"

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
			using ApplicationCallback = eastl::function<void(Event&)>;
			template <typename T>
			using ListenerCallbackFn = eastl::function<bool(T&)>;
			using StoredCallbackFn = eastl::function<bool(Event&)>;
		public:
			EventQueue();
			~EventQueue();

			void Dispatch(Event& e);

			void OnUpdate(Timestep& ts) override;

			void SetEventCallback(ApplicationCallback&& callback);

			template <typename T>
			EventListener AddListener(ListenerCallbackFn<T>&& callback);

			EventListener AddGlobalListener(StoredCallbackFn&& callback);

			void RemoveListener(EventListener& listenerId);

			static Scope<EventQueue> Create();

			template <typename DeleterType, typename AllocatorType>
			static Scope<EventQueue, DeleterType> Create(const AllocatorType* alloc);

		private:
			struct ListenerEntry
			{
				EventListener ListenerId;
				EventType ListenerType;
				StoredCallbackFn CallbackFn;
			};

		private:
			eastl::queue<Scope<Event>> m_WriteQueue;
			eastl::queue<Scope<Event>> m_ReadQueue;
			std::shared_mutex m_WriteMutex;
			std::shared_mutex m_ReadMutex;

			eastl::atomic<uint32_t> m_NextListenerId;
			eastl::vector<ListenerEntry> m_Listeners;
			eastl::hash_map<EventListener, size_t> m_ListenerIndexMap;
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

		template <typename DeleterType, typename AllocatorType>
		Scope<EventQueue, DeleterType> EventQueue::Create(const AllocatorType* alloc)
		{
			return Scope<EventQueue, DeleterType>(new (alloc->allocate(sizeof(EventQueue))) EventQueue(), DeleterType(alloc));
		}

	}

}