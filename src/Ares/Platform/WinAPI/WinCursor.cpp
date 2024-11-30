#include <arespch.h>

#include "Platform/WinAPI/WinCursor.h"

namespace Ares {

	void WinCursor::SetCursorTypeImpl(CursorType type)
	{
		HCURSOR hCursor = nullptr;

		switch (type)
		{
		case CursorType::Arrow: {
			hCursor = LoadCursor(0, IDC_ARROW);
			break;
		}
		case CursorType::TextSelect: {
			hCursor = LoadCursor(0, IDC_IBEAM);
			break;
		}
		case CursorType::Busy: {
			hCursor = LoadCursor(0, IDC_WAIT);
			break;
		}
		case CursorType::DiagResize1: {
			hCursor = LoadCursor(0, IDC_SIZENWSE);
			break;
		}
		case CursorType::DiagResize2: {
			hCursor = LoadCursor(0, IDC_SIZENESW);
			break;
		}
		case CursorType::HResize: {
			hCursor = LoadCursor(0, IDC_SIZEWE);
			break;
		}
		case CursorType::VResize: {
			hCursor = LoadCursor(0, IDC_SIZENS);
			break;
		}
		case CursorType::Move: {
			hCursor = LoadCursor(0, IDC_SIZEALL);
			break;
		}
		case CursorType::LinkSelect: {
			hCursor = LoadCursor(0, IDC_HAND);
			break;
		}
		case CursorType::Loading: {
			hCursor = LoadCursor(0, IDC_APPSTARTING);
			break;
		}
		default: {
			hCursor = LoadCursor(0, IDC_ARROW);
			break;
		}
		}

		if (hCursor != nullptr)
		{
			SetCursor(hCursor);
		}
		else
		{
			AR_CORE_WARN("Failed to load cursor!");
		}
	}

}