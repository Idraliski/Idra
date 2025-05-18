#include "IdraPCH.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Resources/Image/Image.h"
#include "Resources/FileLoader.h"

#include <glad/glad.h>

namespace Idra {
	OpenGLTexture2D::OpenGLTexture2D(const Path& path, bool flipImage)
		: m_Path(path)
	{
		Ref<Image> image = FileLoader::LoadFileAsImage(path, flipImage);

		m_Width = image->GetWidth();
		m_Height = image->GetHeight();

		int channels = image->GetChannels();
		GLenum dataFormat = 0, internalFormat = 0;
		
		if (channels == 1)
		{
			dataFormat = GL_RED;
			internalFormat = GL_R8;
		}
		else if (channels == 2)
		{
			dataFormat = GL_RG;
			internalFormat = GL_RG8;
		}
		else if (channels == 3) 
		{
			dataFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else if (channels == 4)
		{
			dataFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else
		{
			IDRA_CORE_ASSERT(false, "Unknown number of channels!");
			return;
		}

		// OpenGL expects image to start at bottom left, instead of top left
		// image data is flipped vertically in Image class

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// how the texture will be resampled down if it needs to be rendered smaller
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// how the texture will be resampled up if it needs to be rendered larger
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// clamp the wrap modes, S & T is like x and y
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, image->GetData());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}