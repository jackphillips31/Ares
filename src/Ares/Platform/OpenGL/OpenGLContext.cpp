#include <arespch.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Platform/OpenGL/OpenGLContext.h"

namespace Ares {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		AR_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		AR_CORE_INFO("Initializing OpenGL Context");

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		AR_CORE_ASSERT(status, "Failed to initialize GLAD!");

		AR_CORE_INFO("OpenGL Info:");
		AR_CORE_INFO("  {:<10}- {}", "Vendor", (char*)glGetString(GL_VENDOR));
		AR_CORE_INFO("  {:<10}- {}", "Renderer", (char*)glGetString(GL_RENDERER));
		AR_CORE_INFO("  {:<10}- {}", "Version", (char*)glGetString(GL_VERSION));

	#ifdef AR_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		AR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Ares requres at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}