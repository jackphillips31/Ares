#include <arespch.h>
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

#include "Engine/Data/RawData.h"

namespace Ares {

	static GLenum GetUsage(const BufferUsage& usage)
	{
		switch (usage)
		{
		case BufferUsage::Dynamic:	return GL_DYNAMIC_DRAW;
		case BufferUsage::Static:	return GL_STATIC_DRAW;
		}
		return GL_DYNAMIC_DRAW;
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(const size_t size, const uint32_t bindingPoint, const BufferUsage usage)
		: m_BufferSize(size), m_BindingPoint(bindingPoint), m_RendererID(0)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GetUsage(usage));
		glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(bindingPoint), m_RendererID);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(const RawData& data, const uint32_t bindingPoint, const BufferUsage usage)
		: m_BufferSize(data.Size), m_BindingPoint(bindingPoint), m_RendererID(0)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(data.Size), data.Data, GetUsage(usage));
		glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(bindingPoint), m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		if (m_RendererID)
		{
			glDeleteBuffers(1, &m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::SetData(const RawData& data, const BufferUsage usage)
	{
		m_BufferSize = data.Size;
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glNamedBufferData(m_RendererID, m_BufferSize, data.Data, GetUsage(usage));
		glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(data.Size), data.Data, GetUsage(usage));
	}

	void OpenGLUniformBuffer::SetSubData(const RawData& data, const uint32_t offset)
	{
		if (offset + data.Size > m_BufferSize)
		{
			AR_CORE_ASSERT(false, "Buffer Overflow!");
			return;
		}
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(data.Size), data.Data);
	}

}