#include <arespch.h>
#include "Platform/WinAPI/WinOpenGLContext.h"

#include <glad/gl.h>
#include <glad/wgl.h>

#include "Engine/Core/Application.h"

namespace Ares {

	WinOpenGLContext::WinOpenGLContext(void* windowHandle)
		: m_WindowHandle(static_cast<HWND>(windowHandle)), m_Context(nullptr), m_DeviceContext(nullptr)
	{
	}

	void WinOpenGLContext::Init()
	{
		HDC hdc = GetDC(m_WindowHandle);
		AR_CORE_ASSERT(hdc, "Failed to get device context for pixel format!");

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0 || SetPixelFormat(hdc, pixelFormat, &pfd) == 0)
		{
			AR_CORE_ASSERT(false, "Failed to set a compatible pixel format!");
		}

		ReleaseDC(m_WindowHandle, hdc);
		m_DeviceContext = GetDC(m_WindowHandle);

		// Create temporary context
		HGLRC tempContext = 0;
		tempContext = wglCreateContext(m_DeviceContext);
		AR_CORE_ASSERT(tempContext, "Failed to make temporary OpenGL context!");
		wglMakeCurrent(m_DeviceContext, tempContext);

		// Load WGL Extensions
		gladLoaderLoadWGL(m_DeviceContext);

		// DEBUG FLAGS
		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
			0
		};

		// Create the final OpenGL context and get rid of the temporary one
		m_Context = wglCreateContextAttribsARB(m_DeviceContext, 0, attributes);
		AR_CORE_ASSERT(m_Context, "Failed to make final OpenGL context!");

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(tempContext);

		wglMakeCurrent(m_DeviceContext, m_Context);

		// Glad Loader!
		if (!gladLoaderLoadGL())
		{
			AR_CORE_ASSERT(false, "GLAD loader failed!");
		}

		if (!gladLoaderLoadWGL(m_DeviceContext))
		{
			AR_CORE_ASSERT(false, "Failed to initialize WGL extensions with GLAD")
		}

		// Log OpenGL info
		AR_CORE_INFO("OpenGL Info:");
		AR_CORE_INFO("  {:<10}- {}", "Vendor", (char*)glGetString(GL_VENDOR));
		AR_CORE_INFO("  {:<10}- {}", "Renderer", (char*)glGetString(GL_RENDERER));
		AR_CORE_INFO("  {:<10}- {}", "Version", (char*)glGetString(GL_VERSION));

		int maxVertexAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
		AR_CORE_INFO("Max Attribs: {}", maxVertexAttribs);
	}

	void WinOpenGLContext::SwapBuffers()
	{
		::SwapBuffers(m_DeviceContext);
	}

	void WinOpenGLContext::MakeCurrent()
	{
		if (!wglMakeCurrent(m_DeviceContext, m_Context))
		{
			AR_CORE_WARN("Failed to make OpenGL context current!");
		}
	}

}