#pragma once
#include <glad/gl.h>

#include "Engine/Renderer/Assets/Texture.h"

namespace Ares {

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& name, const glm::uvec2& dimensions, const RawData& rawData, const Format format);
		OpenGLTexture(const std::string& name, const RawData& data);
		~OpenGLTexture() override;

		// Core properties
		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetWidth() const override { return static_cast<uint32_t>(m_Width); }
		inline uint32_t GetHeight() const override { return static_cast<uint32_t>(m_Height); }
		inline Format GetFormat() const override { return m_Format; }

		// Renderer ID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return static_cast<uint32_t>(m_RendererID); }

		// Binding and state
		void Bind(uint32_t slot = 0) const override;
		void Unbind() const override;
		inline bool IsBound() const override { return m_BoundSlot != -1; }
		inline uint32_t GetBoundSlot() const override { return m_BoundSlot; }

		// Settings
		void SetFilter(const Filter minFilter, const Filter maxFilter) override;
		void SetWrap(const Wrap wrap) override;

		// Utilities
		void Resize(const uint32_t width, const uint32_t height) override;
		void SetData(const RawData& data) override;
		void GenerateMips() override;
		inline bool IsValid() const override { return m_RendererID; }

	private:
		std::string m_Path;
		std::string m_Name;
		GLsizei m_Width, m_Height;
		Format m_Format;
		mutable int32_t m_BoundSlot;
		GLuint m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;

	private:
		static GLenum GetGLInternalFormat(Format format);
		static GLenum GetGLDataFormat(Format format);
		static GLenum GetGLFilter(Filter filter);
		static GLenum GetGLWrap(Wrap wrap);
		static uint32_t GetBytesPerPixel(Format format);
	};

}