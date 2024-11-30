#include <arespch.h>
#include <glad/gl.h>

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Ares {

	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height)
		: m_FBO(0), m_RBO(0), m_Texture(0), m_Width(width), m_Height(height)
	{
		CreateFramebuffer();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		DestroyFramebuffer();
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == m_Width && height == m_Height)
			return;

		m_Width = width;
		m_Height = height;

		DestroyFramebuffer();
		CreateFramebuffer();
	}

	uintptr_t OpenGLFrameBuffer::GetColorAttachmentHandle() const
	{
		return static_cast<uintptr_t>(m_Texture);
	}

	void OpenGLFrameBuffer::CreateFramebuffer()
	{
		glGenFramebuffers(1, &m_FBO);
		Bind();

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			AR_CORE_ASSERT(false, "Framebuffer is not complete!");
		}

		Unbind();
	}

	void OpenGLFrameBuffer::DestroyFramebuffer()
	{
		if (m_Texture)
		{
			glDeleteTextures(1, &m_Texture);
			m_Texture = 0;
		}
		if (m_RBO)
		{
			glDeleteRenderbuffers(1, &m_RBO);
			m_RBO = 0;
		}
		if (m_FBO)
		{
			glDeleteFramebuffers(1, &m_FBO);
			m_FBO = 0;
		}
	}

}