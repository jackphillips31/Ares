#include <arespch.h>

#include "Engine/Core/Application.h"

#include "Platform/WinAPI/WinOpenGLSharedContext.h"

namespace Ares {

	WinOpenGLSharedContext::WinOpenGLSharedContext(const void* primaryContext)
		: m_HDC(nullptr), m_Context(nullptr)
	{
		Application& app = Application::Get();
		m_HDC = GetDC(static_cast<HWND>(app.GetWindow().GetNativeWindow()));

		m_Context = wglCreateContext(m_HDC);
		if (!m_Context)
		{
			AR_CORE_ASSERT(false, "Failed to create OpenGL context!");
		}
		if (!wglShareLists(reinterpret_cast<HGLRC>(const_cast<void*>(primaryContext)), m_Context))
		{
			AR_CORE_ASSERT(false, "Failed to share OpenGL context!");
		}
	}

	WinOpenGLSharedContext::~WinOpenGLSharedContext()
	{
		if (m_Context)
		{
			wglDeleteContext(m_Context);
		}
	}

	void WinOpenGLSharedContext::MakeCurrent()
	{
		if (!wglMakeCurrent(m_HDC, m_Context))
		{
			AR_CORE_ASSERT(false, "Failed to make OpenGL context current!");
		}
	}

	void WinOpenGLSharedContext::DetachCurrent()
	{
		wglMakeCurrent(nullptr, nullptr);
	}

}