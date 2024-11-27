#pragma once
#include <arespch.h>

namespace Ares {

	namespace WindowSettings {
		enum Flags : uint16_t
		{
			None = 0,
			Windowed =				BIT(0),
			Borderless =			BIT(1),
			Fullscreen =			BIT(2),
			FullscreenExclusive =	BIT(3),
			Resizable =				BIT(4),
			Maximized =				BIT(5),
			DefaultWindow =			0b010001,
			FullscreenBorderless =	0b000110,
			FullscreenWindowed =	0b100001,
		};
	}

}

#define AR_WINDOW_WINDOWED (uint16_t)0b000001
#define AR_WINDOW_BORDERLESS (uint16_t)0b000010
#define AR_WINDOW_FULLSCREEN (uint16_t)0b000100
#define AR_WINDOW_FULLSCREEN_EX (uint16_t)0b001000
#define AR_WINDOW_RESIZABLE (uint16_t)0b010000
#define AR_WINDOW_MAXIMIZED (uint16_t)0b100000
#define AR_WINDOW_DEFAULT_WINDOW (uint16_t)0b010001
#define AR_WINDOW_FULLSCREEN_BORDERLESS (uint16_t)0b000110
#define AR_WINDOW_FULLSCREEN_WINDOWED (uint16_t)0b100001