#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/KeyCodes.h"
#include "Engine/Events/MouseCodes.h"

namespace Ares {

	class Input
	{
	public:
		inline static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }

		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<int32_t, int32_t> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static int32_t GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static int32_t GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static std::pair<int32_t, int32_t> GetMouseClientPosition() { return s_Instance->GetMouseClientPositionImpl(); }
		inline static int32_t GetMouseClientX() { return s_Instance->GetMouseClientXImpl(); }
		inline static int32_t GetMouseClientY() { return s_Instance->GetMouseClientYImpl(); }

		static Scope<Input> Create();
		inline static void Shutdown() { s_Instance.reset(); }

	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<int32_t, int32_t> GetMousePositionImpl() = 0;
		virtual int32_t GetMouseXImpl() = 0;
		virtual int32_t GetMouseYImpl() = 0;
		virtual std::pair<int32_t, int32_t> GetMouseClientPositionImpl() = 0;
		virtual int32_t GetMouseClientXImpl() = 0;
		virtual int32_t GetMouseClientYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}