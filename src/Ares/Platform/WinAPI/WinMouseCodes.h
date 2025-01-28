#pragma once

namespace Ares {

	enum class MouseCode : uint8_t;

	MouseCode WinAPIMouseToMouseCode(uint32_t winApiMouse);
	uint32_t MouseCodeToWinAPIMouse(MouseCode mouseCode);

}