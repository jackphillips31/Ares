#pragma once

namespace Ares::ECS {

	struct Component
	{
		virtual ~Component() = default;
	};

}

#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/Orientation.h"
#include "Engine/ECS/Components/Position.h"