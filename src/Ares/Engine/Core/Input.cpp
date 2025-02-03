#include <arespch.h>
#include "Engine/Core/Input.h"

#include "Engine/Data/MemoryManager.h"

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

	Input* Input::CreatePlatformInput(void* memory, Window* window)
	{
	#if AR_PLATFORM_WINDOWS
		return new (memory) WinInput(window);
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		throw std::runtime_error("Unknown Platform!");
	#endif
		return nullptr;
	}

	/*
	template <typename DeleterType, typename AllocatorType>
	Scope<Input, DeleterType> Input::Create(const AllocatorType& alloc, Window* window)
	{
	#if AR_PLATFORM_WINDOWS
		return Scope<WinInput, DeleterType>(new WinInput(window), DeleterType(alloc));
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		throw std::runtime_error("Unknown Platform!");
	#endif
		return nullptr;
	}
	*/

	//template Scope<Input, Internal::Deleter> Input::Create<Internal::Deleter, Internal::AppAllocator>(const Internal::AppAllocator&, Window*);

}