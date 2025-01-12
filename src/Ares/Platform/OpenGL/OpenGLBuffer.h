#pragma once
#include <glad/gl.h>

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"

namespace Ares {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const RawData& data, const BufferUsage usage);
		~OpenGLVertexBuffer() override;

		// Core properties
		inline size_t GetSize() const override { return m_BufferSize; }
		inline size_t GetCount() const override { return m_BufferCount; }
		inline const BufferLayout& GetBufferLayout() const override { return m_BufferLayout; }

		// Binding and state
		void Bind() const override;
		void Unbind() const override;

		// Setters
		void SetData(const RawData& data, const BufferUsage usage = BufferUsage::Dynamic) override;
		void SetSubData(const RawData& data, const uint32_t offset = 0) override;
		inline void SetBufferLayout(const BufferLayout& layout) override { m_BufferLayout = layout; }

		// RendererID access (for low-level operations)
		inline uint32_t GetRendererID() const { return static_cast<uint32_t>(m_RendererID); }

	private:
		GLuint m_RendererID;
		BufferLayout m_BufferLayout;
		size_t m_BufferCount;
		size_t m_BufferSize;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const RawData& data, const BufferUsage& usage);
		~OpenGLIndexBuffer() override;

		// Core properties
		inline size_t GetSize() const { return m_BufferSize; }
		inline size_t GetCount() const { return m_BufferCount; }

		// Binding and state
		void Bind() const override;
		void Unbind() const override;

		// Setters
		void SetData(const RawData& data, const BufferUsage usage = BufferUsage::Dynamic) override;
		void SetSubData(const RawData& data) override;

		// RendererID access (for low-level operations)
		inline uint32_t GetRendererID() const { return static_cast<uint32_t>(m_RendererID); }

	private:
		GLuint m_RendererID;
		size_t m_BufferCount;
		size_t m_BufferSize;
	};

}