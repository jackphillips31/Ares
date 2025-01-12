#pragma once
#include <glad/gl.h>

#include "Engine/Renderer/VertexArray.h"

namespace Ares {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		// Core properties
		inline const std::vector<VertexBuffer*>& GetVertexBuffers() const { return m_VertexBuffers; }
		inline IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }

		// Binding and state
		void Bind() const override;
		void Unbind() const override;

		// Setters
		void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
		void SetIndexBuffer(IndexBuffer* indexBuffer) override;

		// RendererID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return static_cast<uint32_t>(m_RendererID); }

	private:
		GLuint m_RendererID;
		GLuint m_VertexBufferIndex;
		std::vector<VertexBuffer*> m_VertexBuffers;
		IndexBuffer* m_IndexBuffer;
	};

}