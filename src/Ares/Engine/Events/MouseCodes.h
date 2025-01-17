#pragma once

namespace Ares {

	enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};

	using Mouse = MouseCode;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

}

#define AR_MOUSE_BUTTON_0      ::Ares::Mouse::Button0
#define AR_MOUSE_BUTTON_1      ::Ares::Mouse::Button1
#define AR_MOUSE_BUTTON_2      ::Ares::Mouse::Button2
#define AR_MOUSE_BUTTON_3      ::Ares::Mouse::Button3
#define AR_MOUSE_BUTTON_4      ::Ares::Mouse::Button4
#define AR_MOUSE_BUTTON_5      ::Ares::Mouse::Button5
#define AR_MOUSE_BUTTON_6      ::Ares::Mouse::Button6
#define AR_MOUSE_BUTTON_7      ::Ares::Mouse::Button7
#define AR_MOUSE_BUTTON_LAST   ::Ares::Mouse::ButtonLast
#define AR_MOUSE_BUTTON_LEFT   ::Ares::Mouse::ButtonLeft
#define AR_MOUSE_BUTTON_RIGHT  ::Ares::Mouse::ButtonRight
#define AR_MOUSE_BUTTON_MIDDLE ::Ares::Mouse::ButtonMiddle