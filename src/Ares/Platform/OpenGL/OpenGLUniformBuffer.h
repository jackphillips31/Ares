#pragma once
#include <glad/gl.h>

#include "Engine/Renderer/UniformBuffer.h"

namespace Ares {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const size_t size, const uint32_t bindingPoint, const BufferUsage usage);
		OpenGLUniformBuffer(const RawData& data, const uint32_t bindingPoint, const BufferUsage usage);
		~OpenGLUniformBuffer() override;

		// Core properties
		inline size_t GetSize() const override { return m_BufferSize; }
		inline uint32_t GetBindingPoint() const override { return static_cast<uint32_t>(m_BindingPoint); }

		// Binding and state
		void Bind() const override;
		void Unbind() const override;

		// Setters
		void SetData(const RawData& data, const BufferUsage usage = BufferUsage::Dynamic) override;
		void SetSubData(const RawData& data, const uint32_t offset = 0) override;

		// RendererID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return static_cast<uint32_t>(m_RendererID); }

	private:
		GLuint m_RendererID;
		GLuint m_BindingPoint;
		size_t m_BufferSize;
	};

}