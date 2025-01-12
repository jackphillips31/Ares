#pragma once
#include "Engine/Renderer/GraphicsContext.h"

namespace Ares {

	class WinOpenGLContext : public GraphicsContext
	{
	public:
		WinOpenGLContext(void* windowHandle);

		void Init() override;
		void SwapBuffers() override;
		void MakeCurrent() override;

		inline void* GetContextHandle() const override { return static_cast<void*>(m_Context); }

	private:
		HWND m_WindowHandle;
		HGLRC m_Context;
		HDC m_DeviceContext;
	};

}