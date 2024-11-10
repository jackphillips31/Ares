#include <arespch.h>
#include "Ares/Core/Input.h"

#ifdef AR_PLATFORM_WINDOWS

#endif

namespace Ares {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef AR_PLATFORM_WINDOWS

#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
#endif
	}
}