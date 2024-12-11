#pragma once

#include "Engine/Renderer/GraphicsContext.h"

namespace Ares {

	class WinOpenGLContext : public GraphicsContext
	{
	public:
		WinOpenGLContext(void* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;

		virtual const void* GetContextHandle() const override;

	private:
		HWND m_WindowHandle;
		HGLRC m_Context;
		HDC m_DeviceContext;
	};

}