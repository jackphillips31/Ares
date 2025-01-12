#pragma once
#include <glad/gl.h>

#include "Engine/Renderer/FrameBuffer.h"

namespace Ares {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const uint32_t width, const uint32_t height);
		~OpenGLFrameBuffer();

		// Core properties
		inline uint32_t GetWidth() const override { return static_cast<uint32_t>(m_Width); }
		inline uint32_t GetHeight() const override { return static_cast<uint32_t>(m_Height); }

		// Binding and state
		void Bind() const override;
		void Unbind() const override;

		// Utilities
		void Resize(uint32_t width, uint32_t height) override;

		// TextureID access (for low-level operations)
		inline uintptr_t GetTextureHandle() const override { return static_cast<uintptr_t>(m_Texture); }

	private:
		void CreateFramebuffer();
		void DestroyFramebuffer();

	private:
		GLuint m_FBO;
		GLuint m_RBO;
		GLuint m_Texture;
		GLsizei m_Width, m_Height;
	};

}