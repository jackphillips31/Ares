#include <arespch.h>

#include "Engine/Core/Input.h"

#ifdef AR_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Ares {

	//Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
	#ifdef AR_PLATFORM_WINDOWS
		//return CreateScope<WindowsInput>();
		return nullptr;
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

}