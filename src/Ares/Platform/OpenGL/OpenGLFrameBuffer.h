#pragma once

#include "Engine/Renderer/FrameBuffer.h"

namespace Ares {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		~OpenGLFrameBuffer();

		void Bind() const override;
		void Unbind() const override;
		void Resize(uint32_t width, uint32_t height) override;

		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }

		uintptr_t GetColorAttachmentHandle() const override;

	private:
		uint32_t m_FBO;
		uint32_t m_RBO;
		uint32_t m_Texture;

		uint32_t m_Width;
		uint32_t m_Height;

		void CreateFramebuffer();
		void DestroyFramebuffer();
	};

}