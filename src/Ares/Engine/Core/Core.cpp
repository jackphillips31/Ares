#include <arespch.h>
#include "Engine/Core/Core.h"

#include "Engine/Core/Application.h"
#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares::Internal {

	bool IsApplicationValid()
	{
		return Application::IsValid();
	}

	AppAllocator* GetDefaultAllocator()
	{
		if (Application::IsValid())
		{
			return Application::Get().GetMemoryManager().GetDefaultAllocator();
		}

		return nullptr;
	}

	void* Allocate(size_t size)
	{
		return GetDefaultAllocator()->allocate(size);
	}

	void* Allocate(size_t size, size_t alignment, size_t offset)
	{
		return GetDefaultAllocator()->allocate(size, alignment, offset);
	}

	void Deallocate(void* ptr, size_t size)
	{
		GetDefaultAllocator()->deallocate(ptr, size);
	}

}