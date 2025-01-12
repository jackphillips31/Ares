#pragma once

namespace Ares {

	class Event;
	enum class EventType : uint16_t;
	using EventListener = uint32_t;

	class EventQueue
	{
	public:
		EventQueue() = delete;

		static void Init();
		static void Shutdown();

		template <typename EventTypeTemplate>
		static void Dispatch(EventTypeTemplate e);

		template <typename EventTypeTemplate>
		static EventListener AddListener(std::function<bool(EventTypeTemplate&)> func);

		static void RemoveListener(EventListener& listenerId);
		static void ProcessEvents();

	private:
		static void NotifyListeners(Event& e);
		static void SwapQueues();

	private:
		using EventCallbackFn = std::function<bool(Event&)>;
		struct EventListenerEntry
		{
			uint32_t Id;
			std::function<bool(Event&)> Function;
		};

	private:
		static inline std::queue<Scope<Event>> s_ReadEventQueue;
		static inline std::queue<Scope<Event>> s_WriteEventQueue;
		static inline std::mutex s_ReadQueueMutex;
		static inline std::mutex s_WriteQueueMutex;

		static inline std::atomic<uint32_t> s_NextListenerId{ 1 };
		static inline std::unordered_map<EventType, std::unordered_map<uint32_t, EventCallbackFn>> s_Listeners;
		static inline std::unordered_map<uint32_t, EventType> s_ListenerTypeMap;
		static inline std::mutex s_ListenerMutex;
	};

	template <typename EventTypeTemplate>
	void EventQueue::Dispatch(EventTypeTemplate e)
	{
		std::lock_guard<std::mutex> lock(s_WriteQueueMutex);
		s_WriteEventQueue.emplace(CreateScope<EventTypeTemplate>(e));
	}

	template <typename EventTypeTemplate>
	EventListener EventQueue::AddListener(std::function<bool(EventTypeTemplate&)> func)
	{
		std::lock_guard<std::mutex> lock(s_ListenerMutex);
		uint32_t currentId = s_NextListenerId++;
		s_Listeners[EventTypeTemplate::GetStaticType()][currentId] = [func](Event& e) { return func(static_cast<EventTypeTemplate&>(e)); };
		s_ListenerTypeMap[currentId] = EventTypeTemplate::GetStaticType();
		return currentId;
	}

}