#include <arespch.h>

#ifdef AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinCursor.h"
#endif

#include "Engine/Core/Cursor.h"

namespace Ares {

	Scope<Cursor> Cursor::s_Instance = Cursor::Create();

	Scope<Cursor> Cursor::Create()
	{
	#ifdef AR_PLATFORM_WINDOWS
		return CreateScope<WinCursor>();
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

}