#include <arespch.h>
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Ares {

	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height)
		: m_FBO(0), m_RBO(0), m_Texture(0), m_Width(static_cast<GLsizei>(width)), m_Height(static_cast<GLsizei>(height))
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
		if (static_cast<GLsizei>(width) == m_Width && static_cast<GLsizei>(height) == m_Height)
			return;

		m_Width = static_cast<GLsizei>(width);
		m_Height = static_cast<GLsizei>(height);

		DestroyFramebuffer();
		CreateFramebuffer();
	}

	void OpenGLFrameBuffer::CreateFramebuffer()
	{
		/*
		glGenFramebuffers(1, &m_FBO);
		Bind();

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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
		*/
		glCreateFramebuffers(1, &m_FBO);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
		glTextureStorage2D(m_Texture, 4, GL_RGBA8, m_Width, m_Height);
		glTextureParameteri(m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glNamedFramebufferTexture(m_FBO, GL_COLOR_ATTACHMENT0, m_Texture, 0);

		glCreateRenderbuffers(1, &m_RBO);
		glNamedRenderbufferStorage(m_RBO, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glNamedFramebufferRenderbuffer(m_FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		//glCreateRenderbuffers(1, &m_RBO);
		//glNamedRenderbufferStorage(m_RBO, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		//glNamedFramebufferRenderbuffer(m_FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		//glNamedFramebufferDrawBuffer(m_FBO, GL_COLOR_ATTACHMENT0);

		GLenum status = glCheckNamedFramebufferStatus(m_FBO, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			AR_CORE_CRITICAL("Framebuffer: ERROR {}", status);

			GLint textureBinding = 0;
			glGetNamedFramebufferAttachmentParameteriv(m_FBO, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &textureBinding);
			if (textureBinding == GL_NONE)
			{
				AR_CORE_CRITICAL("Framebuffer: No color attachment found!");
			}
			glGetNamedFramebufferAttachmentParameteriv(m_FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &textureBinding);
			if (textureBinding == GL_NONE)
			{
				AR_CORE_CRITICAL("Framebuffer: No depth attachment found!");
			}
			AR_CORE_ASSERT(false, "Framebuffer: Status is not complete!");
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