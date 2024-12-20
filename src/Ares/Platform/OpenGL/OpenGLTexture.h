#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Engine/Renderer/Texture.h"

namespace Ares {

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& name, const FileBuffer& fileBuffer);
		OpenGLTexture(const std::string& name, const glm::uvec2& dimensions, const Format& format);
		virtual ~OpenGLTexture();

		// Core properties
		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }
		inline Format GetFormat() const override { return m_Format; }

		// Binding and state
		void Bind(uint32_t slot = 0) const override;
		void Unbind() const override;
		inline bool IsBound() const override { return m_BoundSlot != -1; }
		inline uint32_t GetBoundSlot() const override { return m_BoundSlot; }

		// Settings
		void SetFilter(const Filter& minFilter, const Filter& maxFilter) override;
		void SetWrap(const Wrap& wrap) override;

		// Utilities
		void Resize(const uint32_t& width, const uint32_t& height) override;
		void SetData(const void* data, const size_t& size) override;
		void GenerateMips() override;
		inline bool IsValid() const override { return m_RendererID; }

		// Renderer ID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return m_RendererID; }

		inline bool operator==(const Texture& other) const override
		{
			return m_RendererID == static_cast<const OpenGLTexture&>(other).m_RendererID;
		}

	private:
		std::string m_Path;
		std::string m_Name;
		uint32_t m_Width, m_Height;
		Format m_Format;
		mutable int32_t m_BoundSlot;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;

	private:
		static GLenum GetGLInternalFormat(Format format);
		static GLenum GetGLDataFormat(Format format);
		static GLenum GetGLFilter(Filter filter);
		static GLenum GetGLWrap(Wrap wrap);
		static uint32_t GetBytesPerPixel(Format format);
	};

}