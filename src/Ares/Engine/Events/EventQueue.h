/**
 * @file EventQueue.h
 * @brief Defines the EventQueue class for managing event processing.
 * 
 * The EventQueue class is responsible for queuing and dispatching events in the application.
 * It supports adding listeners for specific event types and propagating events to all listeners.
 */
#pragma once

namespace Ares {

	class Event;
	enum class EventType : uint16_t;

	/**
	 * @typedef EventListener
	 * @brief Listener ID stored as a 32 bit unsigned integer.
	 */
	using EventListener = uint32_t;

	/**
	 * @class EventQueue
	 * @brief Handles event queuing and dispatching in the application.
	 * 
	 * The EventQueue stores events and provides mechanisms to dispatch them to listeners.
	 * Listeners can register callbacks for specific event types.
	 */
	class EventQueue
	{
	public:
		/**
		 * @typedef EventListenerCallbackFn
		 * @brief The callback function used for listener callbacks in the EventQueue.
		 * @details It is a boolean function that takes a reference to an Event as the argument.
		 * 
		 * @tparam EventTypeTemplate The type of event.
		 */
		template <typename EventTypeTemplate>
		using EventListenerCallbackFn = std::function<bool(EventTypeTemplate&)>;

		/**
		 * @typedef ApplicationEventCallbackFn
		 * @brief The callback function used for sending events out to the Application.
		 * @details It is a void function that takes a reference to an Event as the argument.
		 */
		using ApplicationEventCallbackFn = std::function<void(Event&)>;
	public:
		EventQueue() = delete;

		/**
		 * @brief Initializes the EventQueue. Called during application startup.
		 */
		static void Init();

		/**
		 * @brief Shuts down the EventQueue. Called during application shutdown.
		 */
		static void Shutdown();

		/**
		 * @brief Processes all events currently in the queue.
		 * This method should be called during the application's main loop.
		 */
		static void OnUpdate();

		/**
		 * @brief Dispatches an event to the EventQueue.
		 * @tparam EventTypeTemplate The type of event being dispatched.
		 * @param event The event to dispatch.
		 */
		template <typename EventTypeTemplate>
		static void Dispatch(EventTypeTemplate e);

		/**
		 * @brief Adds a listener for a specific event type.
		 * @tparam EventTypeTemplate The type of event to listen for.
		 * @param func The callback function to invoke when the event is processed.
		 * @return The ID of the created listener.
		 * 
		 * Example usage:
		 * ```cpp
		 * EventListener listener = EventQueue::AddListener<WindowResizeEvent>([](WindowResizeEvent& event) {
		 *    // Handle event
		 *    return true;
		 * });
		 * ```
		 */
		template <typename EventTypeTemplate>
		static EventListener AddListener(EventListenerCallbackFn<EventTypeTemplate>&& func);

		/**
		 * @brief Removes a listener by its ID.
		 * @param listenerId The ID of the listener to remove.
		 * 
		 * Example usage:
		 * ```cpp
		 * EventListener listener = EventQueue::AddListener<WindowResizeEvent>([](WindowResizeEvent& event) {});
		 * EventQueue::RemoveListener(listener);
		 * ```
		 */
		static void RemoveListener(EventListener& listenerId);

		/**
		 * @brief Sets the callback in which the EventQueue sends Events to.
		 * @param callback The callback function to invoke when events are processed.
		 */
		static void SetEventCallback(ApplicationEventCallbackFn&& callback);

	private:
		/**
		 * @brief Notifies all listeners of a given event.
		 * @param e The event to notify listeners about.
		 */
		static void NotifyListeners(Event& e);

		/**
		 * @brief Swaps the read and write queues to process new events.
		 */
		static void SwapQueues();

	private:
		/**
		 * typedef EventCallbackFn
		 * @brief Alias for an event callback function type.
		 * 
		 * Defines the signature for callback function that process events.
		 * The function takes an `Event&` as a parameter and returns `bool` indicating
		 * whether the event was successfully handled (`true`) or not (`false`).
		 */
		using EventCallbackFn = std::function<bool(Event&)>;

		/**
		 * @struct EventListenerEntry
		 * @brief Represents an entry for an event listener.
		 * 
		 * This structure is used to store information about an individual event listener,
		 * including its unique identifier and the callback function it uses to process events.
		 */
		struct EventListenerEntry
		{
			uint32_t Id;															///< Unique ID of the listener.
			std::function<bool(Event&)> Function;									///< The callback function associated with this listener.
		};

	private:
		static inline std::queue<Scope<Event>> s_ReadEventQueue;					///< Queue of events ready for processing. 
		static inline std::queue<Scope<Event>> s_WriteEventQueue;					///< Queue of events waiting to be processed.
		static inline std::mutex s_ReadQueueMutex;									///< Mutex for the read queue.
		static inline std::mutex s_WriteQueueMutex;									///< Mutex for the write queue.

		static inline std::atomic<uint32_t> s_NextListenerId{ 1 };					///< Next unique listener ID.
		static inline std::unordered_map<EventType, std::unordered_map<uint32_t, EventCallbackFn>> s_Listeners;	///< Map of event types to their registered listeners.
		static inline std::unordered_map<uint32_t, EventType> s_ListenerTypeMap;	///< Map of listener IDs to event types.
		static inline std::mutex s_ListenerMutex;									///< Mutex for listener operations.
		static inline std::mutex s_CallbackMutex;									///< Mutex for Application Event Callback.
		static inline ApplicationEventCallbackFn s_Callback = nullptr;				///< Callback function that events are sent to during processing.
	};

	// Template method implementations

	template <typename EventTypeTemplate>
	void EventQueue::Dispatch(EventTypeTemplate e)
	{
		std::lock_guard<std::mutex> lock(s_WriteQueueMutex);
		s_WriteEventQueue.emplace(CreateScope<EventTypeTemplate>(e));
	}

	template <typename EventTypeTemplate>
	EventListener EventQueue::AddListener(EventListenerCallbackFn<EventTypeTemplate>&& func)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		uint32_t currentId = s_NextListenerId++;
		s_Listeners[EventTypeTemplate::GetStaticType()][currentId] = [func = std::move(func)](Event& e) { return func(static_cast<EventTypeTemplate&>(e)); };
		s_ListenerTypeMap[currentId] = EventTypeTemplate::GetStaticType();
		return currentId;
	}

}