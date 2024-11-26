# Ares Game Engine

Ares is a simple, extensible game engine designed to eventually support both 2D and 3D game development. Currently, Ares supports rendering ImGui elements, window management with customizable startup options, and an event system. The engine is designed to grow with your project and will eventually include full 2D and 3D rendering capabilities.

## Table of Contents
1. [Introduction](#ares-game-engine)
2. [Table of Contents](#table-of-contents)
3. [Key Features](#key-features)
4. [Installation](#installation)
   - [Prerequisites](#prerequisites)
   - [Cloning the Repo](#cloning-the-repository)
   - [Building the Project](#building-the-project)
5. [Creating a New Blank Project](#creating-a-new-blank-project)
6. [Usage](#usage)
   - [Create a New Application](#create-a-new-application)
   - [Set Window Style (Optional)](#set-window-style-optional)
   - [Creating and Adding Layers](#creating-and-adding-layers)
   - [Handling Events](#handling-events)
   - [Logging](#logging)
   - [Rendering](#rendering)
   - [Complete Example Code](#complete-code)
7. [License](#license)
8. [Contact](#contact)

## Key Features
- **ImGui Rendering**: Currently supports rendering ImGui UI elements.
- **Window Management**: Allows for window style customization:
  - Windowed, Borderless, Fullscreen, Fullscreen Exclusive (changes resolution)
  - Resizable and maximized windows.
- **Event System**: Supports event handling with custom layers and user-defined event functions.
- **Logging**: Built-in logging with various severity levels (trace, info, warn, error, critical).
- **Extensibility**: Easily add layers to manage different parts of your game or application.

## Installation

### Prerequisites
- **Premake 5**: You’ll need [Premake 5](https://premake.github.io/download) to generate project files. 
- **Git**: To clone the repository.

### Cloning the Repository
```bash
git clone https://github.com/yourusername/ares.git
cd ares
```

### Building the Project
1. Download and install [Premake 5](https://premake.github.io/download) if you haven't already.
2. In the root directory of the cloned repository, run Premake to generate project files for your development environment:
   ```bash
   premake5 <your_environment>
   ```
   Replace `<your_environment>` with your desired build system, such as `vs2022` for Visual Studio 2022 or `gmake` for GNU Make.

3. Open the generated project in your IDE and build the solution.

## Creating a New Blank Project

The current `premake5.lua` is set up with the main project named **"Sandbox"**. If you'd like to create a new blank project, such as **"MyApplication"**, follow these steps:

1. **Update `premake5.lua`**:  
   Open the `premake5.lua` file and replace all instances of `"Sandbox"` with your desired project name (e.g., `"MyApplication"`). One of these instances defines the location for the new project, so Premake will automatically create a new directory for it during the next step.

2. **Regenerate Project Files**:  
   Run Premake to generate project files for your new application by running the following command:
   ```bash
   premake5 <your_environment>
   ```

3. **Start Working**:  
   A new directory (e.g., `src/MyApplication`) will be automatically created for your project. This will give you a blank slate to start working on your new application.

## Usage

### Create a New Application
In your `MyApplication.cpp`, include Ares and the necessary entry point:
```cpp
// MyApplication.cpp
#include <Ares.h>
#include <Engine/Core/EntryPoint.h>

class MyApplication : public Ares::Application
{
public:
  MyApplication()
  {
     // Constructor code
  }
  ~MyApplication()
  {
     // Destructor code
  }
};

Ares::Application* Ares::CreateApplication()
{
  // Set startup settings here before
  // before returning MyApplication()!
  return new MyApplication();
}
```

### Set Window Style (Optional)
You can customize the startup window style with macros:
```cpp
Ares::Application::SetStartupSettings(AR_WINDOW_BORDERLESS);
```
 
<details>
 <summary>MyApplication.cpp complete code.</summary>
 
 ```c++
 // MyApplication.cpp
 #include <Ares.h>
 #include <Engine/Core/EntryPoint.h>

 class MyApplication : public Ares::Application
 {
 public:
    MyApplication()
    {
       // Constructor code
    }
    ~MyApplication()
    {
       // Destructor code
    }
 };

 Ares::Application* Ares::CreateApplication()
 {
    Ares::Application::SetStartupSettings(AR_WINDOW_DEFAULT_WINDOW);
    return new MyApplication();
 }
 ```
</details>
 
Here’s a list of available window styles:
 
| Value | Description |
|--|--|
| `AR_WINDOW_WINDOWED`<br>`Ares::WindowSettings::Windowed` | A standard window with a title bar and borders. |
| `AR_WINDOW_BORDERLESS`<br>`Ares::WindowSettings::Borderless` | A window without borders or a title bar, often used for fullscreen-like experiences but within a window. The user cannot resize or move the window. |
| `AR_WINDOW_FULLSCREEN`<br>`Ares::WindowSettings::Fullscreen` | A window that covers the entire screen without borders. This style is often used for immersive fullscreen applications while still allowing the user to switch to other applications. |
| `AR_WINDOW_FULLSCREEN_EX`<br>`Ares::WindowSettings::FullscreenExclusive` | A fullscreen window that takes exclusive control of the screen, changing the screen resolution. Unlike the standard fullscreen, this style can provide better performance by bypassing some windowing system overhead. |
| `AR_WINDOW_RESIZABLE`<br>`Ares::WindowSettings::Resizable` | Allows a `AR_WINDOW_WINDOWED` to be resized. |
| `AR_WINDOW_MAXIMIZED`<br>`Ares::WindowSettings::Maximized` | A window that starts in a maximized state. If the `AR_WINDOW_BORDERLESS` flag is set, then this will make the window fullscreen. |
| `AR_WINDOW_DEFAULT_WINDOW`<br>`Ares::WindowSettings::DefaultWindow` | A combination of `AR_WINDOW_WINDOWED` and `AR_WINDOW_RESIZABLE`. The default window style. This style is used when no specific window settings are provided or when set to 0. |
| `AR_WINDOW_FULLSCREEN_BORDERLESS`<br>`Ares::WindowSettings::FullscreenBorderless` | A combination of `AR_WINDOW_BORDERLESS` and `AR_WINDOW_FULLSCREEN`, where the window is fullscreen without borders but still behaves like a normal window in terms of interaction with the operating system. |
| `AR_WINDOW_FULLSCREEN_WINDOWED`<br>`Ares::WindowSettings::FullscreenWindowed` | A combination of `AR_WINDOW_WINDOWED` and `AR_WINDOW_MAXIMIZED`, where the window is maximized to cover most of the screen but retains the windowed nature with the ability to move but not resize. |

### Creating and Adding Layers
Create a file `MyLayer.h` with a new class that inherits from `Ares::Layer`:
```cpp
// MyLayer.h
#pragma once
#include <Ares.h>

class MyLayer : public Ares::Layer
{
public:
  MyLayer(); 
  virtual ~MyLayer() = default; 

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  void OnUpdate(Ares::Timestep ts) override;
  void OnEvent(Ares::Event& e) override;
  virtual void OnImGuiRender() override;
};
```

<details>
 <summary>MyLayer.cpp complete code.</summary>

 ```c++
 // MyLayer.cpp
 #include "MyLayer.h"

 MyLayer::MyLayer()
    : Layer("MyLayer")
 {
 }

 void MyLayer::OnAttach()
 {
 }

 void MyLayer::OnDetach()
 {
 }

 void MyLayer::OnUpdate(Ares::Timestep ts)
 {
 }

 void MyLayer::OnEvent(Ares::Event& e)
 {
 }

 void MyLayer::OnImGuiRender()
 {
 }
 ```
</details>

### Push Layer to Application
In `MyApplication.cpp`, push the layer:
```cpp
// MyApplication.cpp
MyApplication::MyApplication()
{
  PushLayer(new MyLayer());
}
```
   
<details>
  <summary>MyApplication.cpp complete code.</summary>

  ```c++
  // MyApplication.cpp
  #include <Ares.h>
  #include <Engine/Core/EntryPoint.h>

  #include "MyLayer.h"

  class MyApplication : public Ares::Application
  {
  public:
     MyApplication()
     {
        // Constructor code
        PushLayer(new MyLayer());
     }
     ~MyApplication()
     {
        // Destructor code
     }
  };

  Ares::Application* Ares::CreateApplication()
  {
     Ares::Application::SetStartupSettings(AR_WINDOW_DEFAULT_WINDOW);
     return new MyApplication();
  }
  ```
</details>

### Handling Events
Inside your layer, you can handle specific events, such as a key press:
```cpp
// MyLayer.cpp
void MyLayer::OnEvent(Ares::Event& e)
{
   Ares::EventDispatcher dispatcher(e);
   dispatcher.Dispatch<Ares::KeyPressedEvent>(AR_BIND_EVENT_FN(MyLayer::OnKeyPressed));
}

bool MyLayer::OnKeyPressed(Ares::KeyPressedEvent& e)
{
   if (e.GetKeyCode() == Ares::KeyCode::Space)
   {
      // Code for Space key
   }
   return false;
}
```

<details>
 <summary>MyLayer.h complete code.</summary>

 ```c++
 // MyLayer.h
 #pragma once
 #include <Ares.h>

 class MyLayer : public Ares::Layer
 {
 public:
    MyLayer(); 
    virtual ~MyLayer() = default; 

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(Ares::Timestep ts) override;
    void OnEvent(Ares::Event& e) override;
    virtual void OnImGuiRender() override;

 private:
    bool OnKeyPressed(Ares::KeyPressedEvent& e);
 };
 ```
</details>
<details>
 <summary>MyLayer.cpp complete code.</summary>

 ```c++
 // MyLayer.cpp
 #include "MyLayer.h"

 MyLayer::MyLayer()
    : Layer("MyLayer")
 {
 }

 void MyLayer::OnAttach()
 {
 }

 void MyLayer::OnDetach()
 {
 }

 void MyLayer::OnUpdate(Ares::Timestep ts)
 {
 }

 void MyLayer::OnEvent(Ares::Event& e)
 {
    Ares::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Ares::KeyPressedEvent>(AR_BIND_EVENT_FN(MyLayer::OnKeyPressed));
 }

 void MyLayer::OnImGuiRender()
 {
 }

 bool MyLayer::OnKeyPressed(Ares::KeyPressedEvent& e)
 {
    if (e.GetKeyCode() == AR_KEY_SPACE)
    {
       // Code for Space key
    }
    return false;
 }
 ```
</details>

### Logging
Ares includes robust logging functionality to help developers track application behavior and debug efficiently. Logging in Ares is accessed through a set of predefined macros that support various log levels. These macros are simple to use and accept inputs in a format compatible with the fmt library, allowing for powerful and flexible message formatting.

**Available Logging Macros:**
| Macro | Definition |
|--|--|
| `AR_TRACE`    | For detailed information, typically used during development to trace program execution. |
| `AR_BUG`      | Used for debugging. |
| `AR_INFO`     | For general informational messages that highlight the progress of the application. |
| `AR_WARN`     | For warning messages that indicate potentially harmful situations. |
| `AR_ERROR`    | For error messages that indicate a significant problem, though the application can continue running. |
| `AR_CRITICAL` | For critical issues that likely require immediate attention or cause the application to terminate. |

**Example Usage**
```c++
int someValue = 42;
std::string someString = "Ares Engine";

// Log a trace message with formatted values
AR_TRACE("SomeValue: {} - Message: {}", someValue, someString);

// Log an error
AR_ERROR("An error occurred with value: {}", someValue);
```
In the example above:
- `{}` placeholders are replaced with the corresponding variables, just as with the `fmt` library.
- This approach ensures that logging macros are flexible, concise, and easy to read.

**Console or Windowed Application:**<br>
Logging messages are displayed differently based on how the application is configured:
- **Console Applications**: Logs are printed directly to the console window, providing immediate visibility.
- **Windowed Applications**: Logs can be rendered to an ImGui-based console, allowing developers to view logs just as easily as with a Console Application.

**Integrating the ImGui Console:**<br>
To enable logging in a **Windowed Application**, add the following code to the `OnImGuiRender()` method in `MyLayer.cpp`.
```c++
// MyLayer.cpp
void MyLayer::OnImGuiRender()
{
   Ares::Log::GetConsole()->Draw("Console", true);
}
```
This creates an ImGui window named `"Console"` where all log messages are displayed.

**Advanced Use Case**:<br>
You can also add logic to toggle the visibility of the ImGui console. For example:
1. Add a member variable to track visibility:
  ```c++
  // MyLayer.h
  ...
  private:
     bool m_ShowConsole;
  ...
  ```
2. Toggle its state with a key press (e.g., the grave accent key "`"):
  ```c++
  // MyLayer.cpp
  bool MyLayer::OnKeyPressed(Ares::KeyPressedEvent& e)
  {
     if (e.GetKeyCode() == Ares::KeyCode::GraveAccent)
     {
        m_ShowConsole = !m_ShowConsole;
     }
     return false;
  }
  ```
3. Use the variable to conditionally render the console:
  ```c++
  // MyLayer.cpp
  void MyLayer::OnImGuiRender()
  {
     Ares::Log::GetConsole()->Draw("Console", m_ShowConsole);
  }
  ```
By incorporating logging into your application, you can efficiently monitor runtime behavior, debug issues, and gain insight into the engine's internal operations.

<details>
 <summary>MyLayer.h complete code.</summary>

 ```c++
 // MyLayer.h
 #pragma once
 #include <Ares.h>

 class MyLayer : public Ares::Layer
 {
 public:
    MyLayer(); 
    virtual ~MyLayer() = default; 

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(Ares::Timestep ts) override;
    void OnEvent(Ares::Event& e) override;
    virtual void OnImGuiRender() override;

 private:
    bool OnKeyPressed(Ares::KeyPressedEvent& e);
    bool m_ShowConsole;
 };
 ```
</details>
<details>
 <summary>MyLayer.cpp complete code.</summary>

 ```c++
 // MyLayer.cpp
 #include "MyLayer.h"

 MyLayer::MyLayer()
    : Layer("MyLayer"), m_ShowConsole(false)
 {
    int someValue = 42;
    std::string someString = "Ares Engine";

    // Log a trace message with formatted values
    AR_TRACE("SomeValue: {} - Message: {}", someValue, someString);

    // Log an error
    AR_ERROR("An error occurred with value: {}", someValue);
 }

 void MyLayer::OnAttach()
 {
 }

 void MyLayer::OnDetach()
 {
 }

 void MyLayer::OnUpdate(Ares::Timestep ts)
 {
 }

 void MyLayer::OnEvent(Ares::Event& e)
 {
    Ares::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Ares::KeyPressedEvent>(AR_BIND_EVENT_FN(MyLayer::OnKeyPressed));
 }

 void MyLayer::OnImGuiRender()
 {
    Ares::Log::GetConsole()->Draw("Console", m_ShowConsole);
 }

 bool MyLayer::OnKeyPressed(Ares::KeyPressedEvent& e)
 {
    if (e.GetKeyCode() == Ares::KeyCode::GraveAccent)
    {
       m_ShowConsole = !m_ShowConsole;
    }
    return false;
 }
 ```
</details>

### Rendering
Ares currently includes basic rendering functionality. While it's still in the early stages, these features lay the groundwork for more advanced rendering capabilities to be added in the future.<br>

**Current Features**:
- `Ares::RenderCommand::SetClearColor`:
 Sets the clear color for the OpenGL viewport. You can specify a color using an RGBA vector, like so:
 ```c++
 Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
 ```
- `Ares::RenderCommand::Clear`:
 Clears the OpenGL viewport buffers.
 ```c++
 Ares::RenderCommand::Clear();
 ```
These commands can be used in your layer's `OnUpdate()` function to define the background color and refresh the viewport each frame.

**Example Usage**:
```c++
void MyLayer::OnUpdate(Ares::Timestep ts)
{
   Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
   Ares::RenderCommand::Clear();
}
```
This may not seem like much yet, but these functions are just the beginning. The rendering system will continue to expand to include features for rendering 2D and 3D objects, lighting, shaders, and more. Stay tuned for future updates!

### Complete Code
<details>
 <summary>MyApplication.cpp</summary>

 ```c++
 // MyApplication.cpp
 #include <Ares.h>
 #include <Engine/Core/EntryPoint.h>

 #include "MyLayer.h"

 class MyApplication : public Ares::Application
 {
 public:
    MyApplication()
    {
       // Constructor code
       PushLayer(new MyLayer());
    }
    ~MyApplication()
    {
       // Destructor code
    }
 };

 Ares::Application* Ares::CreateApplication()
 {
    Ares::Application::SetStartupSettings(AR_WINDOW_DEFAULT_WINDOW);
    return new MyApplication();
 }
 ```
</details>
<details>
 <summary>MyLayer.h</summary>

 ```c++
 // MyLayer.h
 #pragma once
 #include <Ares.h>

 class MyLayer : public Ares::Layer
 {
 public:
    MyLayer(); 
    virtual ~MyLayer() = default; 

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(Ares::Timestep ts) override;
    void OnEvent(Ares::Event& e) override;
    virtual void OnImGuiRender() override;

 private:
    bool OnKeyPressed(Ares::KeyPressedEvent& e);
    bool m_ShowConsole;
 };
 ```
</details>
<details>
 <summary>MyLayer.cpp</summary>

 ```c++
 // MyLayer.cpp
 #include "MyLayer.h"

 MyLayer::MyLayer()
    : Layer("MyLayer"), m_ShowConsole(false)
 {
    int someValue = 42;
    std::string someString = "Ares Engine";

    // Log a trace message with formatted values
    AR_TRACE("SomeValue: {} - Message: {}", someValue, someString);

    // Log an error
    AR_ERROR("An error occurred with value: {}", someValue);
 }

 void MyLayer::OnAttach()
 {
 }

 void MyLayer::OnDetach()
 {
 }

 void MyLayer::OnUpdate(Ares::Timestep ts)
 {
    Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Ares::RenderCommand::Clear();
 }

 void MyLayer::OnEvent(Ares::Event& e)
 {
    Ares::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Ares::KeyPressedEvent>(AR_BIND_EVENT_FN(MyLayer::OnKeyPressed));
 }

 void MyLayer::OnImGuiRender()
 {
    Ares::Log::GetConsole()->Draw("Console", m_ShowConsole);
 }

 bool MyLayer::OnKeyPressed(Ares::KeyPressedEvent& e)
 {
    if (e.GetKeyCode() == Ares::KeyCode::GraveAccent)
    {
       m_ShowConsole = !m_ShowConsole;
    }
 }
 ```
</details>

## License
This project is licensed under the MIT License.

## Contact
For issues or feature requests, please create a new issue on the GitHub repository.
