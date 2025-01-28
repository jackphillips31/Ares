#include <arespch.h>
#include "Engine/Core/Input.h"

#if AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinInput.h"
#endif

namespace Ares::Systems {

	Scope<Input> Input::Create(Window* window)
	{
	#if AR_PLATFORM_WINDOWS
		return Scope<WinInput>(new WinInput(window));
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		throw std::runtime_error("Unknown Platform!");
	#endif
		return nullptr;
	}

}