#pragma once
#include <arespch.h>

namespace Ares {

	enum class WindowFlags : uint32_t
	{
		None					= 0b0000,
		Borderless				= 0b0001,
		Fullscreen				= 0b0010,
		BorderlessFullscreen	= 0b0011,
	};

	// Enable bitwise operations for WindowFlags
	inline WindowFlags operator |(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(
			static_cast<std::underlying_type_t<WindowFlags>>(lhs) |
			static_cast<std::underlying_type_t<WindowFlags>>(rhs)
		);
	}

	inline WindowFlags& operator |=(WindowFlags& lhs, WindowFlags rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	inline WindowFlags operator &(WindowFlags lhs, WindowFlags rhs)
	{
		return static_cast<WindowFlags>(
			static_cast<std::underlying_type_t<WindowFlags>>(lhs) &
			static_cast<std::underlying_type_t<WindowFlags>>(rhs)
		);
	}

	inline WindowFlags operator ~(WindowFlags flags)
	{
		return static_cast<WindowFlags>(~static_cast<std::underlying_type_t<WindowFlags>>(flags));
	}

	inline bool hasFlag(WindowFlags value, WindowFlags flag)
	{
		return (value & flag) == flag;
	}

}

#define AR_WINDOW_BORDERLESS 0b0001
#define AR_WINDOW_FULLSCREEN 0b0010
#define AR_WINDOW_FULLSCREENBORDERLESS 0b0011