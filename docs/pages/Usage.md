@page usage Usage
# Create a New Application
To set up an application with Ares, you need to implement the Ares::CreateApplication function. This function is where you'll configure your application's settings and add custom layers.
## Basic Application Setup
Create a file, e.g., `MyApplication.cpp`, and include the necessary headers:
<br>
**Example**:

```cpp
// MyApplication.cpp
#include <Ares.h>
#include <Engine/Core/EntryPoint.h>

Ares::Application* Ares::CreateApplication()
{
	Ares::Application* app = new Ares::Application();
	return app;
}
```
## Customizing Application Settings
You can customize your application's name, window style, dimensions, updates per second and more using Ares::ApplicationSettings:
<br>
**Example**:

```cpp
// MyApplication.cpp
#include <Ares.h>
#include <Engine/Core/EntryPoint.h>

Ares::Application* Ares::CreateApplication()
{
	Ares::ApplicationSettings settings;
	settings.Name = "My Application";
	settings.ThreadCount = 4;
	settings.WindowStyle = AR_WINDOW_WINDOWED | AR_WINDOW_RESIZABLE;

	Ares::Application* app = new Ares::Application(settings);
	return app;
}
```
@note Available window styles can be found [here](#Ares::WindowSettings).
# Adding Layers
To add functionality, create a custom layer class that inherits from Ares::Layer. For example, in `MyLayer.h`:
<br>
**Example**:

```cpp
// MyLayer.h
#pragma once
#include <Ares.h>

class MyLayer : public Ares::Layer
{
public:
	MyLayer() = default;
	~MyLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Ares::Timestep ts) override;
};
```
Push the Layer in `MyApplication.cpp`:
```cpp
#include <Ares.h>
#include <Engine/Core/EntryPoint.h>
#include "MyLayer.h"

Ares::Application* Ares::CreateApplication()
{
	Ares::ApplicationSettings settings;
	settings.Name = "My Application";
	settings.ThreadCount = 4;
	settings.WindowStyle = AR_WINDOW_WINDOWED | AR_WINDOW_RESIZABLE;

	Ares::Ref<MyLayer> layer = Ares::CreateRef<MyLayer>();

	Ares::Application* app = new Ares::Application(settings);
	app->PushLayer(layer);
	return app;
}
```
You have now successfully set up your first application using the Ares Engine!

<div class="section_buttons">
|Previous|Next|
|:-|-:|
|[Getting Started](#getting_started)|[Event System](#event_system)|
</div>