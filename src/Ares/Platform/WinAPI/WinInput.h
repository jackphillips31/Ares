#pragma once
#include "Engine/Core/Input.h"

namespace Ares::Systems {

	class WinInput : public Input
	{
	public:
		~WinInput() override;

		bool IsKeyPressed(KeyCode key) override;
		bool IsMouseButtonPressed(MouseCode button) override;
		MousePosition GetMousePosition() override;
		MousePosition GetMouseClientPosition() override;

	private:
		WinInput(Window* window);
		friend class Input;
		template <typename ObjectType, typename... Args>
		friend AppScope<ObjectType> Ares::CreateAppScope(Args&&... args);

	private:
		HWND m_Window;
	};

}