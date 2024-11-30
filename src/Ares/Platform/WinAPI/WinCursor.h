#pragma once

#include "Engine/Core/Cursor.h"

namespace Ares {

	class WinCursor : public Cursor
	{
	protected:
		virtual void SetCursorTypeImpl(CursorType type);
	};

}