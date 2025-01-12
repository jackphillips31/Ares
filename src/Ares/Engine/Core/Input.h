#pragma once
#include <glm/vec2.hpp>

namespace Ares {

	enum class KeyCode : uint16_t;
	enum class MouseCode : uint16_t;

	class Input
	{
	public:
		inline static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }

		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static glm::ivec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static int32_t GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static int32_t GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static glm::ivec2 GetMouseClientPosition() { return s_Instance->GetMouseClientPositionImpl(); }
		inline static int32_t GetMouseClientX() { return s_Instance->GetMouseClientXImpl(); }
		inline static int32_t GetMouseClientY() { return s_Instance->GetMouseClientYImpl(); }

		static Scope<Input> Create();
		inline static void Shutdown() { s_Instance.reset(); }

	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual glm::ivec2 GetMousePositionImpl() = 0;
		virtual int32_t GetMouseXImpl() = 0;
		virtual int32_t GetMouseYImpl() = 0;
		virtual glm::ivec2 GetMouseClientPositionImpl() = 0;
		virtual int32_t GetMouseClientXImpl() = 0;
		virtual int32_t GetMouseClientYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}