#include <arespch.h>
#include <stb_image.h>

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Ares {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& name, const void* data, const size_t size)
		: m_Name(name)
	{
		int32_t width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* imageData = stbi_load_from_memory(static_cast<const uint8_t*>(data), static_cast<int32_t>(size), &width, &height, &channels, 0);
		AR_CORE_ASSERT(imageData, "Failed to load image!");
		m_Width = static_cast<uint32_t>(width);
		m_Height = static_cast<uint32_t>(height);

		uint32_t internalFormat = 0;
		uint32_t dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		AR_CORE_ASSERT(internalFormat & dataFormat, "Fromat not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}