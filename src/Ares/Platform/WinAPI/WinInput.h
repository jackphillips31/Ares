#pragma once

#include "Engine/Core/Input.h"

namespace Ares {

	class WinInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) override;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual std::pair<int32_t, int32_t> GetMousePositionImpl() override;
		virtual int32_t GetMouseXImpl() override;
		virtual int32_t GetMouseYImpl() override;
		virtual std::pair<int32_t, int32_t> GetMouseClientPositionImpl() override;
		virtual int32_t GetMouseClientXImpl() override;
		virtual int32_t GetMouseClientYImpl() override;
	};

}