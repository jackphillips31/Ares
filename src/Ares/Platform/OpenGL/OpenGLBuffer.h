#pragma once

#include <glad/gl.h>

#include "Engine/Renderer/Buffer.h"

namespace Ares {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const RawData& data);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const RawData& data) const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual bool operator==(const VertexBuffer& other) const override
		{
			return m_RendererID == ((OpenGLVertexBuffer&)other).m_RendererID;
		}

	private:
		GLuint m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count);
		OpenGLIndexBuffer(const RawData& data);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const RawData& data) const override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		GLuint m_RendererID;
		mutable uint32_t m_Count;
	};

}