#pragma once
#include <EASTL/variant.h>

#include "Engine/Data/MemoryManager/AppAllocator.h"

namespace Ares {

	template <class... Types>
	using Variant = eastl::variant<Types...>;

}