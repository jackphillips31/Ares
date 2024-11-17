#include <arespch.h>

#include <glad/gl.h>
#include <glad/wgl.h>

#include "Platform/WinAPI/WinOpenGLContext.h"
#include "Engine/Core/Application.h"

namespace Ares {

	HGLRC WinOpenGLContext::s_SharedContext = nullptr;

	WinOpenGLContext::WinOpenGLContext(void* windowHandle)
		: m_WindowHandle((HWND)windowHandle), m_Context(NULL)
	{
	}

	void WinOpenGLContext::Init()
	{
		HDC hdc = GetDC(m_WindowHandle);
		if (!hdc)
		{
			AR_CORE_ASSERT(false, "Failed to get device context for OpenGL!");
		}

		// Set up a pixel format for the window
		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0 || SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
		{
			AR_CORE_ASSERT(false, "Failed to set a compatible pixel format!");
		}

		// Create and enable a temporary (helper) opengl context
		HGLRC tempContext = NULL;
		if (NULL == (tempContext = wglCreateContext(hdc)))
		{
			AR_CORE_ASSERT(false, "Failed to create the initial rendering context!");
		}
		wglMakeCurrent(hdc, tempContext);

		// Load WGL Extensions
		gladLoaderLoadWGL(hdc);

		// Set the desired OpenGL Version
		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		// Create the final opengl context and get rid of the temporary one
		if (s_SharedContext == nullptr)
		{
			if (NULL == (m_Context = wglCreateContext(hdc)))
			{
				AR_CORE_ASSERT(false, "Failed to create the final rendering context!");
			}
			s_SharedContext = m_Context;
		}
		else
		{
			if (NULL == (m_Context = wglCreateContextAttribsARB(hdc, s_SharedContext, attributes)))
			{
				AR_CORE_ASSERT(false, "Failed to create the final rendering shared context!");
			}
		}
		
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hdc, m_Context);

		// Glad Loader!
		if (!gladLoaderLoadGL())
		{
			AR_CORE_ASSERT(false, "Glad Loader failed!");
		}

		if (!gladLoaderLoadWGL(hdc))
		{
			AR_CORE_ASSERT(false, "Failed to initialize WGL extensions with GLAD")
		}

		// Log OpenGL info
		AR_CORE_INFO("OpenGL Info:");
		AR_CORE_INFO("  {:<10}- {}", "Vendor", (char*)glGetString(GL_VENDOR));
		AR_CORE_INFO("  {:<10}- {}", "Renderer", (char*)glGetString(GL_RENDERER));
		AR_CORE_INFO("  {:<10}- {}", "Version", (char*)glGetString(GL_VERSION));

		ReleaseDC(m_WindowHandle, hdc);
	}

	void WinOpenGLContext::SwapBuffers()
	{
		HDC hdc = GetDC(m_WindowHandle);
		::SwapBuffers(hdc);
		ReleaseDC(m_WindowHandle, hdc);
	}

}