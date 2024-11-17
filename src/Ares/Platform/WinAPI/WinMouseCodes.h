#pragma once

#include "Engine/Events/MouseCodes.h"

namespace Ares {

	MouseCode WinAPIMouseToMouseCode(uint32_t winApiMouse);
	uint32_t MouseCodeToWinAPIMouse(MouseCode mouseCode);

}