#pragma once

#include "Engine/Core/Core.h"

namespace Ares {

	enum class CursorType : uint32_t
	{
		Arrow = 0,
		TextSelect,
		Busy,
		DiagResize1,
		DiagResize2,
		HResize,
		VResize,
		Move,
		LinkSelect,
		Loading,
	};

	class Cursor
	{
	public:
		inline static void SetCursorType(CursorType type) { s_Instance->SetCursorTypeImpl(type); }

		static Scope<Cursor> Create();
		inline static void Shutdown() { s_Instance.reset(); }
	protected:
		virtual void SetCursorTypeImpl(CursorType type) = 0;

	private:
		static Scope<Cursor> s_Instance;
	};

}