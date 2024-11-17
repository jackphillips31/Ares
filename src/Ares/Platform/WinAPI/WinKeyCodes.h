#pragma once

#include "Engine/Events/KeyCodes.h"

namespace Ares {

	KeyCode WinAPIKeyToKeyCode(uint32_t winApiKey);
	uint32_t KeyCodeToWinAPIKey(KeyCode keyCode);

}