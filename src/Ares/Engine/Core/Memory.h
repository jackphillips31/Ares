#pragma once

namespace Ares {

	class Application;

	template <typename ObjectType>
	using AppScope = eastl::unique_ptr<ObjectType, eastl::function<void(void*)>>;

	template <typename ObjectType, typename... Args>
	AppScope<ObjectType> CreateAppScope(Args&&... args)
	{
		if (Application::Get().IsValid())
		{
			Internal::AppAllocator* alloc = Application::Get().GetMemoryManager().GetDefaultAllocator();
			void* memory = alloc->allocate(sizeof(ObjectType));
			ObjectType* result = new (memory) ObjectType(std::forward<Args>(args)...);

			eastl::function<void(void*)> deleter = [alloc](void* ptr)
				{
					ObjectType* obj = static_cast<ObjectType*>(ptr);
					obj->~ObjectType();
					alloc->deallocate(ptr, sizeof(ObjectType));
				};

			return AppScope<ObjectType>(result, deleter);
		}
		else
		{
			ObjectType* result = new ObjectType(std::forward<Args>(args)...);

			eastl::function<void(void*)> deleter = [](void* ptr)
				{
					ObjectType* obj = static_cast<ObjectType*>(ptr);
					obj->~ObjectType();
					delete obj;
				};

			return AppScope<ObjectType>(result, deleter);
		}
	}

}

#include "Engine/Core/Application.h"