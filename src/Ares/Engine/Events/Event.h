/**
 * @file Event.h
 * @brief Defines the base Event class and event-related utilities.
 * 
 * @details Events are used to communicate between different parts of the engine, such as
 * user input, window events, and application lifecycle events.
 */
#pragma once

namespace Ares {

	/**
	 * @enum EventType
	 * @brief Represents the various types of events in the application.
	 * 
	 * @details This enumeration defines all possible event types that can occur within the
	 * application, such as window events, application lifecycle events, input events,
	 * and asset-related events.
	 */
	enum class EventType : uint16_t
	{
		None = 0,			///< No event.
		WindowClose,		///< The window is being closed.
		WindowResize,		///< The window has been resized.
		WindowFocus,		///< The window has gained focus.
		WindowLostFocus,	///< The window has lost focus.
		WindowMoved,		///< The window has been moved.
		AppTick,			///< An application tick event.
		AppUpdate,			///< An application update event.
		AppRender,			///< An application render event.
		KeyPressed,			///< A key has been pressed.
		KeyReleased,		///< A key has been released.
		KeyTyped,			///< A key has been typed.
		MouseButtonPressed,	///< A mouse button has been pressed.
		MouseButtonReleased,///< A mouse button has been released.
		MouseMoved,			///< A mouse has been moved.
		MouseScrolled,		///< A mouse wheel has been scrolled.
		AssetUpdate,		///< An asset has been updated.
		AssetStaged,		///< An asset has been staged.
		AssetLoading,		///< An asset is being loaded.
		AssetLoaded,		///< An asset has been successfully loaded.
		AssetFailed,		///< An asset failed to load.
		AssetUnloaded,		///< An asset has been unloaded.
		AssetUnstaged		///< An asset has been unstaged.
	};

	/**
	 * @enum EventCategory
	 * @brief Represents categories for classifying events.
	 * 
	 * @details This enumeration defines bitwise categories used to classify events in the application.
	 * Categories can be combined using bitwise OR to allow for flexible event filtering and handling.
	 */
	enum EventCategory : uint16_t
	{
		None = 0,								///< No category.
		EventCategoryApplication	= BIT(0),	///< Events related to the application, such as lifecycle events.
		EventCategoryInput			= BIT(1),	///< Events related to user input (keyboard or mouse).
		EventCategoryKeyboard		= BIT(2),	///< Events specifically related to keyboard input.
		EventCategoryMouse			= BIT(3),	///< Events specifically related to mouse movement or input.
		EventCategoryMouseButton	= BIT(4),	///< Events specifically related to mouse button actions.
		EventCategoryAsset			= BIT(5)	///< Events specifically related to assets.
	};

	/**
	 * @def EVENT_CLASS_TYPE
	 * @brief A utility macro to define the event type.
	 */
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
		virtual EventType GetEventType() const override { return GetStaticType(); }\
		virtual const char* GetName() const override { return #type; }

	/**
	 * @def EVENT_CLASS_CATEGORY
	 * @brief A utility macro to define the event category.
	 */
	#define EVENT_CLASS_CATEGORY(category) virtual uint16_t GetCategoryFlags() const override { return category; }

	/**
	 * @class Event
	 * @brief Base class for all events in the engine.
	 * 
	 * @details Events are propagated through the application to allow systems to respond to
	 * user input, window changes, and other triggers.
	 */
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;

		/**
		 * @brief Indicates whether the event has been handled.
		 */
		bool Handled = false;

		/**
		 * @brief Retrieves the type of event.
		 * 
		 * @return The event type.
		 */
		virtual EventType GetEventType() const = 0;

		/**
		 * @brief Retrieves the name of the event.
		 * 
		 * @return The name of the event.
		 */
		virtual const char* GetName() const = 0;

		/**
		 * @brief Retrieves the category flags of the event.
		 * 
		 * @return The event category flags.
		 */
		virtual uint16_t GetCategoryFlags() const = 0;

		/**
		 * @brief Retrieves a string with event related information or the event name.
		 * 
		 * @return The event string.
		 */
		virtual std::string ToString() const { return GetName(); }

		/**
		 * @brief Tells whether an event is in a given category.
		 * 
		 * @param category The event category to test against.
		 * @return `true` if an event has that category flag set; otherwise, `false`.
		 */
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	/**
	 * @class EventDispatcher
	 * @brief A utility class for dispatching events to appropriate handlers based on event type.
	 * 
	 * @details The EventDispatcher simplifies event handling by allowing a specific function to
	 * to be executed only if the event type matches the type of the dispatched event.
	 * 
	 * **Example Usage**:
	 * ```cpp
	 * void MyLayer::OnEvent(Ares::Event& event)
	 * {
	 *     Ares::EventDispatcher dispatcher(event);
	 * 
	 *     dispatcher.Dispatch<WindowResizeEvent>(AR_BIND_EVENT_FN(Layer::OnWindowResize));
	 * }
	 * 
	 * bool MyLayer::OnWindowResize(Ares::WindowResizeEvent& event)
	 * {
	 *     // Handle resize logic, return true if event is handled
	 *     return true;
	 * }
	 * ```
	 */
	class EventDispatcher
	{
		/**
		 * @typedef EventFn
		 * @brief Alias for a function type that takes an event of type T and returns a boolean.
		 * 
		 * @tparam T The type of event the function will handle.
		 */
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		/**
		 * @brief Constructs an EventDispatcher with the given event reference.
		 * 
		 * @param event A reference to the event to be dispatched.
		 */
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		/**
		 * @brief Dispatches the event to the provided function if the event type matches.
		 * 
		 * @details The function is called only if the type of the event matches the template parameter `T`.
		 * If the event is handled by the function, the event's `Handled` flag is set to true.
		 * 
		 * @tparam T The type of event to be handled.
		 * @tparam F The type of the handler function.
		 * @param func The function to call if the event type matches.
		 * It should take a reference to `T` and return a boolean indicating whether the event was handled.
		 * @return `true` if the event type matches and the function is called; otherwise, `false`.
		 */
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;		///< Reference to the event being dispatched.
	};

	/**
	 * @anchor event_ostream_operator
	 * @brief Outputs the string representation of an Event object to an output stream.
	 * 
	 * @details This operator allows an Event to be directly output to an `std::ostream`.
	 * 
	 * @param os The output stream to write to.
	 * @param e The event to output.
	 * @return A reference to the output stream.
	 */
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}