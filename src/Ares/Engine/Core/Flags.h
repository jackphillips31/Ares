/**
 * @file Flags.h
 * @brief Contains window configuration flags for the application.
 * 
 * @details This file defines various window settings flags that are used to specify the behavior
 * and appearance of the window when creating a new window for the application. The flags
 * can be combined to set properties such as windowed mode, fullscreen mode, borderless,
 * resizable, etc. The file also defines convenient constants for the most common window
 * settings.
 */
#pragma once

namespace Ares {

	/**
	 * @namespace Ares::WindowSettings
	 * @brief Contains settings and flags related to window creation and configuration.
	 * 
	 * @details This namespace defines various flags that can be used to configure the behavior and
	 * appearance of the window. The flags are used to specify window properties such as windowed mode,
	 * fullscreen, and resizability.
	 */
	namespace WindowSettings {

		/**
		 * @enum Flags
		 * @brief A set of flags used to configure window settings.
		 * 
		 * @details These flags can be combined to set various properties for the window such as windowed
		 * or fullscreen mode, borderless, and whether the window is resizable or maximized. The
		 * flags use bitwise operations for easy combination.
		 */
		enum Flags : uint16_t
		{
			None = 0,							///< No flags set.
			Windowed =				BIT(0),		///< Windowed mode flag.
			Borderless =			BIT(1),		///< Borderless window flag.
			Fullscreen =			BIT(2),		///< Fullscreen mode flag.
			FullscreenExclusive =	BIT(3),		///< Fullscreen exclusive mode flag.
			Resizable =				BIT(4),		///< Resizable window flag.
			Maximized =				BIT(5),		///< Maximized window flag.
			DefaultWindow =			0b010001,	///< Default window settings flags (Windowed + Resizable).
			FullscreenBorderless =	0b000110,	///< Fullscreen borderless window flags (Borderless + Fullscreen).
			FullscreenWindowed =	0b100001,	///< Fullscreen windowed mode flags (Windowed + Maximized).
		};
	}

}

/**
 * @def AR_WINDOW_WINDOWED
 * @brief Windowed mode flag.
 * @details Creates a window with borders.
 */
#define AR_WINDOW_WINDOWED (uint16_t)0b000001
/**
 * @def AR_WINDOW_BORDERLESS
 * @brief Borderless window flag.
 * @details Creates a window without borders.
 */
#define AR_WINDOW_BORDERLESS (uint16_t)0b000010
/**
 * @def AR_WINDOW_FULLSCREEN
 * @brief Fullscreen mode flag.
 * @details Creates a window that takes up the whole screen, if possible.
 */
#define AR_WINDOW_FULLSCREEN (uint16_t)0b000100
/**
 * @def AR_WINDOW_FULLSCREEN_EX
 * @brief Fullscreen exclusive mode flag.
 * @details Changes the displays resolution to the size of the window, if possible.
 */
#define AR_WINDOW_FULLSCREEN_EX (uint16_t)0b001000
/**
 * @def AR_WINDOW_RESIZABLE
 * @brief Resizable window flag.
 * @details Creates a window that is resizable.
 */
#define AR_WINDOW_RESIZABLE (uint16_t)0b010000
/**
 * @def AR_WINDOW_MAXIMIZED
 * @brief Maximized window flag.
 * @details Creates a window that is maximized on startup.
 */
#define AR_WINDOW_MAXIMIZED (uint16_t)0b100000
/**
 * @def AR_WINDOW_DEFAULT_WINDOW
 * @brief Default window flags.
 * @details A combination of @ref AR_WINDOW_WINDOWED and @ref AR_WINDOW_RESIZABLE.
 */
#define AR_WINDOW_DEFAULT_WINDOW (uint16_t)0b010001
/**
 * @def AR_WINDOW_FULLSCREEN_BORDERLESS
 * @brief Fullscreen borderless window flags.
 * @details A combination of @ref AR_WINDOW_BORDERLESS and @ref AR_WINDOW_FULLSCREEN.
 */
#define AR_WINDOW_FULLSCREEN_BORDERLESS (uint16_t)0b000110
/**
 * @def AR_WINDOW_FULLSCREEN_WINDOWED
 * @brief Fullscreen windowed flags.
 * @details A combination of @ref AR_WINDOW_WINDOWED and @ref AR_WINDOW_MAXIMIZED.
 */
#define AR_WINDOW_FULLSCREEN_WINDOWED (uint16_t)0b100001