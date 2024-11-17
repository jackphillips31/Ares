#pragma once

#include "Engine/Renderer/GraphicsContext.h"

namespace Ares {

	class WinOpenGLContext : public GraphicsContext
	{
	public:
		WinOpenGLContext(void* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		HWND m_WindowHandle;
		HGLRC m_Context;

		static HGLRC s_SharedContext;
	};

}