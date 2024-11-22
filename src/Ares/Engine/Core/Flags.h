#pragma once
#include <arespch.h>

namespace Ares {

	namespace WindowSettings {
		enum Flags : uint16_t
		{
			None = 0,
			Borderless = BIT(0),
			Fullscreen = BIT(1),
			FullscreenBorderless = 0b0011,
		};
	}

}

#define AR_WINDOW_DEFAULT (uint16_t)0b0000
#define AR_WINDOW_BORDERLESS (uint16_t)0b0001
#define AR_WINDOW_FULLSCREEN (uint16_t)0b0010
#define AR_WINDOW_FULLSCREEN_BORDERLESS (uint16_t)0b0011