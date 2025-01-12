#pragma once

namespace Ares {

	enum class KeyCode : uint16_t;

	KeyCode WinAPIKeyToKeyCode(uint32_t winApiKey);
	uint32_t KeyCodeToWinAPIKey(KeyCode keyCode);

}