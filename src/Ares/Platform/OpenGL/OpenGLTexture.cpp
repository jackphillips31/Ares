#include <arespch.h>
#include "Platform/OpenGL/OpenGLTexture.h"

#include <glm/vec2.hpp>
#include <stb_image.h>

#include "Engine/Data/RawData.h"

namespace Ares {

	OpenGLTexture::OpenGLTexture(const std::string& name, const glm::uvec2& dimensions, const RawData& rawData, const Format format)
		: m_Name(name), m_Width(static_cast<GLsizei>(dimensions.x)), m_Height(static_cast<GLsizei>(dimensions.y)), m_Format(format), m_RendererID(0), m_BoundSlot(-1)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		m_InternalFormat = GetGLInternalFormat(format);
		m_DataFormat = GetGLDataFormat(format);

		// Allocate immutable storage for the texture
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		// Set the raw data
		SetData(rawData);

		// Set default texture parameters
		SetFilter(Filter::Linear, Filter::Linear);
		SetWrap(Wrap::Repeat);
	}

	OpenGLTexture::OpenGLTexture(const std::string& name, const RawData& data)
		: m_Name(name), m_Format(Format::None), m_RendererID(0), m_BoundSlot(-1)
	{
		const size_t dataSize = data.Size;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		if (dataSize == 3 || dataSize == 4)
		{
			m_Width = 1;
			m_Height = 1;

			if (dataSize == 3)
				m_Format = Format::RGB;
			if (dataSize == 4)
				m_Format = Format::RGBA;

			m_InternalFormat = GetGLInternalFormat(m_Format);
			m_DataFormat = GetGLDataFormat(m_Format);

			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
		}
		else
		{
			int32_t width, height, channels;
			stbi_set_flip_vertically_on_load(1);

			stbi_uc* imageData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data.Data), static_cast<int32_t>(data.Size), &width, &height, &channels, 0);
			AR_CORE_ASSERT(imageData, "Failed to load image!");
			m_Width = static_cast<GLsizei>(width);
			m_Height = static_cast<GLsizei>(height);

			if (channels == 4)
			{
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGBA;
				m_Format = Format::RGBA;
			}
			else if (channels == 3)
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
				m_Format = Format::RGB;
			}

			AR_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, static_cast<void*>(imageData));

			stbi_image_free(imageData);
		}

		// Set default texture parameters
		SetFilter(Filter::Linear, Filter::Linear);
		SetWrap(Wrap::Repeat);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		if (m_RendererID)
		{
			glDeleteTextures(1, &m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		AR_CORE_ASSERT(m_RendererID != 0, "Invalid Renderer ID!");
		glBindTextureUnit(static_cast<GLuint>(slot), m_RendererID);
		m_BoundSlot = static_cast<int32_t>(slot);
	}

	void OpenGLTexture::Unbind() const
	{
		if (m_BoundSlot != -1)
		{
			glBindTextureUnit(static_cast<GLuint>(m_BoundSlot), 0);
			m_BoundSlot = -1;
		}
	}

	void OpenGLTexture::SetFilter(const Filter minFilter, const Filter maxFilter)
	{
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetGLFilter(minFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetGLFilter(maxFilter));
	}

	void OpenGLTexture::SetWrap(const Wrap wrap)
	{
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetGLWrap(wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetGLWrap(wrap));
	}

	void OpenGLTexture::Resize(const uint32_t width, const uint32_t height)
	{
		m_Width = static_cast<GLsizei>(width);
		m_Height = static_cast<GLsizei>(height);

		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
	}

	void OpenGLTexture::SetData(const RawData& data)
	{
		AR_CORE_ASSERT(
			static_cast<uint32_t>(data.Size) ==
			static_cast<uint32_t>(m_Width) * static_cast<uint32_t>(m_Height) * GetBytesPerPixel(m_Format),
			"Data size mismatch!"
		);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void OpenGLTexture::GenerateMips()
	{
		glGenerateTextureMipmap(m_RendererID);
	}

	GLenum OpenGLTexture::GetGLInternalFormat(Format format)
	{
		switch (format)
		{
		case Format::RGB: return GL_RGB8;
		case Format::RGBA: return GL_RGBA8;
		case Format::RGBA16F: return GL_RGBA16F;
		case Format::RGBA32F: return GL_RGBA32F;
		case Format::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
		default: AR_CORE_ASSERT(false, "Unsupported format!"); return 0;
		}
	}

	GLenum OpenGLTexture::GetGLDataFormat(Format format)
	{
		switch (format)
		{
		case Format::RGB: return GL_RGB;
		case Format::RGBA: return GL_RGBA;
		case Format::Depth24Stencil8: return GL_DEPTH_STENCIL;
		default: AR_CORE_ASSERT(false, "Unsupported format!"); return 0;
		}
	}

	GLenum OpenGLTexture::GetGLFilter(Filter filter)
	{
		switch (filter)
		{
		case Filter::Linear: return GL_LINEAR;
		case Filter::Nearest: return GL_NEAREST;
		default: AR_CORE_ASSERT(false, "Unsupported filter!"); return 0;
		}
	}

	GLenum OpenGLTexture::GetGLWrap(Wrap wrap)
	{
		switch (wrap)
		{
		case Wrap::Repeat: return GL_REPEAT;
		case Wrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case Wrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
		case Wrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
		default: AR_CORE_ASSERT(false, "Unsupported wrap!"); return 0;
		}
	}

	uint32_t OpenGLTexture::GetBytesPerPixel(Format format)
	{
		switch (format)
		{
		case Format::RGB: return 3;
		case Format::RGBA: return 4;
		case Format::RGBA16F: return 8;
		case Format::RGBA32F: return 16;
		case Format::Depth24Stencil8: return 4;
		default: AR_CORE_ASSERT(false, "Unsupported format!"); return 0;
		}
	}

}