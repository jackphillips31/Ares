#include <arespch.h>
#include "Engine/Core/Window.h"

#if AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinWindow.h"
#endif

namespace Ares {

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#if AR_PLATFORM_WINDOWS
		return CreateScope<WinWindow>(props);
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

}