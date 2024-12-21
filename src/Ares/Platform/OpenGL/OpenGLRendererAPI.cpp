#include <arespch.h>
#include <glad/gl.h>

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ares {

	void OpenGLMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void* userParam
	)
	{
		std::string errorMessage(static_cast<const char*>(message));
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH: AR_CORE_CRITICAL(errorMessage); return;
		case GL_DEBUG_SEVERITY_MEDIUM: AR_CORE_ERROR(errorMessage); return;
		case GL_DEBUG_SEVERITY_LOW: AR_CORE_WARN(errorMessage); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: AR_CORE_INFO(errorMessage); return;
		}

		AR_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		AR_CORE_INFO("Initializing OpenGLRendererAPI");
		
	#ifdef AR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(static_cast<GLfloat>(color.r), static_cast<GLfloat>(color.g), static_cast<GLfloat>(color.b), static_cast<GLfloat>(color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Finish() {
		glFinish();
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		GLsizei count = static_cast<GLsizei>(indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount());
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

}