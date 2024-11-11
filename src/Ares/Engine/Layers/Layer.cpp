#include <arespch.h>

#include "Engine/Layers/Layer.h"

namespace Ares {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}

}