#pragma once
#include <EASTL/vector.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	template <typename Type>
	using Vector = eastl::vector<Type, Internal::AppAllocator>;

}