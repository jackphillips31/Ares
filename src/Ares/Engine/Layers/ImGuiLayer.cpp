#include <arespch.h>
#include "Engine/Layers/ImGuiLayer.h"

#ifdef AR_PLATFORM_WINDOWS
#include "Platform/WinAPI/WinImGuiLayer.h"
#endif

namespace Ares {

	Ref<ImGuiLayer> ImGuiLayer::Create()
	{
#	ifdef AR_PLATFORM_WINDOWS
		return CreateRef<WinImGuiLayer>();
	#else
		AR_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif
	}

	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name)
	{
	}

}