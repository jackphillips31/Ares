#pragma once
#include <EASTL/string.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	using String = eastl::basic_string<char, Internal::AppAllocator>;

}

namespace eastl {

	template<>
	struct hash<Ares::String>
	{
		size_t operator()(const Ares::String& string) const
		{
			return eastl::hash<const char*>()(string.c_str());
		}
	};

}