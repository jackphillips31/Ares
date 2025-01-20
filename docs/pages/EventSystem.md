@page event_system Event System
@tableofcontents
# Overview
The **Event System** in Ares provides a powerful and flexible way to manage and process events throughout your application. It allows you to queue events, add listeners for specific event types, and dispatch events efficiently, ensuring clear separation of concerns and scalability.

The [EventQueue](#Ares::EventQueue) class lies at the core of the event system. It provides static methods for:
 - Dispatching events to the queue.
 - Adding and removing listeners.
 - Processing events during the main application loop.

The [EventQueue](#Ares::EventQueue) works in tandem with the Ares [Application](#Ares::Application). The [EventQueue](#Ares::EventQueue) is initialized and shutdown within the [Application](#Ares::Application) class's [constructor](#Ares::Application::Application) and [destructor](#Ares::Application::~Application). Events regarding the window are dispatched to the [EventQueue](#Ares::EventQueue) from the platform specific window implementation. In the [Application](#Ares::Application) class's `Run` method, [EventQueue::ProcessEvents](#Ares::EventQueue::ProcessEvents) is called. When [ProcessEvents](#Ares::EventQueue::ProcessEvents) is called all the events in the queue are sent to the [Application](#Ares::Application) class's `OnEvent` method, which in turn hands off the events to all the [Layers](#Ares::Layer) in the [Application](#Ares::Application). If the event is not handled within the various OnEvent methods, then the [EventQueue](#Ares::EventQueue) will send the event to any [listener callbacks](#Ares::EventQueue::EventListenerCallbackFn) for that event type.
# Key Features
 - **Thread-Safe Queues**: Uses mutexes to ensure event handling is thread-safe.
 - **Customizable Callbacks**: Listeners register their own callback function for event handling.
 - **Efficient Processing**: Events are queued for asynchronous processing, maintaining application performance.
# How to Use the Event System
## Initialization and Shutdown
Initialization and shutdown is handled within the Application class. If you are creating your own application logic, you will need to call these methods yourself.
<br>
**Example**:

```cpp
#include <Engine/Events/EventQueue.h>

int main() {
	// Initialize the event queue
	Ares::EventQueue::Init();

	// Your code here

	// Shutdown the event queue
	Ares::EventQueue::Shutdown();
	return 0;
}
```

## Processing Events
Processing events is handled within the [Application](#Ares::Application) class. If you are creating your own application logic, you will need to set the [EventQueue's](#Ares::EventQueue) callback function and call the [OnUpdate](#Ares::Application::OnUpdate) method yourself.
@note If you do not set the [EventQueue's](#Ares::EventQueue) event callback, then events will only be sent out to registered listeners!

**Example**:

```cpp
#include <Engine/Events/EventQueue.h>

int main() {
	// Initialize the event queue
	Ares::EventQueue::Init();
	Ares::EventQueue::SetEventCallback(std::bind(&OnEvent, this, std::placeholders::_1));

	while (true)
	{
		// Process events in the queue
		Ares::EventQueue::OnUpdate();
	}

	// Shutdown the event queue
	Ares::EventQueue::Shutdown();
}

void OnEvent(Ares::Event& event)
{
	// Process events
}
```
Alternatively, there is a predefined [macro](#AR_BIND_EVENT_FN) you can use to bind the function rather than `std::bind`.
```cpp
Ares::EventQueue::SetEventCallback(AR_BIND_EVENT_FN(OnEvent));
```

## Dispatching Events
To dispatch an event, use the [Dispatch](#Ares::EventQueue::Dispatch) method with your event type.
<br>
**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

WindowResizeEvent resizeEvent(800, 600);
EventQueue::Dispatch<WindowResizeEvent>(resizeEvent);
```
## Adding Listeners
Register listeners for specific event types with the [AddListener](#Ares::EventQueue::AddListener) method. The callback function will be executed whenever the event type is processed.
@note The Event class has an operator that allows it to be used directly with **std::ostream**!<br>[Documentation](\ref event_ostream_operator)

**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

EventListener listener = EventQueue::AddListener<WindowResizeEvent>([](WindowResizeEvent& event) {
	std::cout << event << std::endl; // This will print: "WindowResizeEvent: width, height"
	return true;                     // Return true if the event was successfully handled.
});
```
## Removing Listeners
When a listener is no longer needed, remove it using the [RemoveListener](#Ares::EventQueue::RemoveListener) method.
<br>
**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

EventQueue::RemoveListener(listener);
```
# Relevant Documentation
 - Ares::EventQueue
 - Ares::EventListener
 - Ares::Event

<div class="section_buttons">
|Previous|Next|
|:-|-:|
|[Usage](#usage)|[Asset Manager](#asset_manager)|
</div>