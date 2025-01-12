#include <arespch.h>
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"

namespace Ares {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		}

		AR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: m_VertexBufferIndex(0), m_IndexBuffer(nullptr), m_RendererID(0)
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		if (m_RendererID)
		{
			glDeleteVertexArrays(1, &m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
	{
		if (vertexBuffer == nullptr)
		{
			AR_CORE_ASSERT(false, "Tried to add nullptr VertexBuffer!");
			return;
		}

		const auto& layout = vertexBuffer->GetBufferLayout();
		AR_CORE_ASSERT(layout.GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		// Check if the vertex buffer is empty
		if (vertexBuffer->GetSize() == 0)
		{
			// Iterate through the layout and disable attributes
			for (const BufferElement& element : layout)
			{
				if (element.DataType == ShaderDataType::Mat3 || element.DataType == ShaderDataType::Mat4)
				{
					uint32_t count = 0;
					switch (element.DataType)
					{
					case ShaderDataType::Mat3: count = 3; break;
					case ShaderDataType::Mat4: count = 4; break;
					}
					for (uint32_t i = 0; i < count; i++)
					{
						glDisableVertexAttribArray(m_VertexBufferIndex + i);
						glVertexAttrib4f(m_VertexBufferIndex + i, 0.0f, 0.0f, 0.0f, 1.0f);
					}
					m_VertexBufferIndex += count;
				}
				else
				{
					glDisableVertexAttribArray(m_VertexBufferIndex);
					switch (element.GetComponentCount())
					{
					case 1: glVertexAttrib1f(m_VertexBufferIndex, 0.0f);
					case 2: glVertexAttrib2f(m_VertexBufferIndex, 0.0f, 0.0f);
					case 3: glVertexAttrib3f(m_VertexBufferIndex, 0.0f, 0.0f, 0.0f);
					case 4: glVertexAttrib4f(m_VertexBufferIndex, 0.0f, 0.0f, 0.0f, 1.0f);
					}
					m_VertexBufferIndex++;
				}
			}
			m_VertexBuffers.push_back(vertexBuffer);
			return;
		}

		// Add attributes for non-empty buffers
		for (const BufferElement& element : layout)
		{
			if (element.DataType == ShaderDataType::Mat3 || element.DataType == ShaderDataType::Mat4)
			{
				uint32_t count = 0;
				switch (element.DataType)
				{
				case ShaderDataType::Mat3: count = 3; break;
				case ShaderDataType::Mat4: count = 4;
				}
				for (uint32_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex + i);
					glVertexAttribPointer(
						static_cast<GLuint>(m_VertexBufferIndex + i),
						static_cast<GLint>(count),
						ShaderDataTypeToOpenGLBaseType(element.DataType),
						element.Normalized ? GL_TRUE : GL_FALSE,
						static_cast<GLsizei>(layout.GetStride()),
						reinterpret_cast<const void*>(element.Offset + element.UnitSize * count * i)
					);
					if (element.Instanced)
						glVertexAttribDivisor(m_VertexBufferIndex + i, 1);
				}

				m_VertexBufferIndex += count;
				continue;
			}

			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(
				static_cast<GLuint>(m_VertexBufferIndex),
				static_cast<GLint>(element.GetComponentCount()),
				ShaderDataTypeToOpenGLBaseType(element.DataType),
				element.Normalized ? GL_TRUE : GL_FALSE,
				static_cast<GLsizei>(layout.GetStride()),
				reinterpret_cast<const void*>(element.Offset)
			);
			if (element.Instanced)
				glVertexAttribDivisor(m_VertexBufferIndex, 1);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
	{
		if (indexBuffer == nullptr)
		{
			AR_CORE_ASSERT(false, "Tried to add nullptr IndexBuffer!");
			return;
		}

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}