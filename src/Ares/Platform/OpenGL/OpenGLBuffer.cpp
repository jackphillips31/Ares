#include <arespch.h>
#include "Platform/OpenGL/OpenGLBuffer.h"

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

	//--------------------------------------------------------------
	//------------------------ Vertex Buffer -----------------------
	//--------------------------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(const RawData& data, const BufferUsage usage)
		: m_BufferCount(data.Size / sizeof(float)), m_BufferSize(data.Size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.Size), data.Data, GetUsage(usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const RawData& data, const BufferUsage usage)
	{
		m_BufferCount = data.Size / sizeof(float);
		m_BufferSize = data.Size;
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.Size), data.Data, GetUsage(usage));
	}

	void OpenGLVertexBuffer::SetSubData(const RawData& data, const uint32_t offset)
	{
		if (data.Size > m_BufferSize)
		{
			AR_CORE_ASSERT(false, "Buffer is not big enough!");
			return;
		}
		m_BufferCount = data.Size / sizeof(float);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(data.Size), data.Data);
	}

	//--------------------------------------------------------------
	//------------------------ Index Buffer ------------------------
	//--------------------------------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(const RawData& data, const BufferUsage& usage)
		: m_BufferCount(data.Size / sizeof(uint32_t)), m_BufferSize(data.Size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.Size), data.Data, GetUsage(usage));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::SetData(const RawData& data, const BufferUsage usage)
	{
		m_BufferCount = data.Size / sizeof(uint32_t);
		m_BufferSize = data.Size;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.Size), data.Data, GetUsage(usage));
	}

	void OpenGLIndexBuffer::SetSubData(const RawData& data)
	{
		if (data.Size > m_BufferSize)
		{
			AR_CORE_ASSERT(false, "Buffer is not big enough!");
			return;
		}
		m_BufferCount = data.Size / sizeof(uint32_t);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(data.Size), data.Data);
	}

}