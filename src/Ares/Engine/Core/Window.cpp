#include <arespch.h>

#include "Engine/Core/Window.h"

#ifdef AR_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Ares {

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef AR_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

}