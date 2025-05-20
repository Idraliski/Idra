#include "IdraPCH.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Resources/Image/Image.h"

#include <glad/glad.h>

namespace Idra {
	void GetChannelFormat(int channels, unsigned int& dataFormat, unsigned int& internalFormat)
	{
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
	}

	OpenGLTexture2D::OpenGLTexture2D(const Path& path, bool flipImage)
		: m_Path(path)
	{
		Image image(path, flipImage);

		m_Width = image.GetWidth();
		m_Height = image.GetHeight();

		int channels = image.GetChannels();
		GLenum dataFormat = 0, internalFormat = 0;
		GetChannelFormat(channels, dataFormat, internalFormat);

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

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, image.GetData());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::vector<Path>& paths, bool flipImage)
		: m_Paths(paths)
	{
		IDRA_CORE_ASSERT(paths.size() == 6, "Cube map must have 6 images!");

		bool initialized = false;
		GLenum dataFormat = 0, internalFormat = 0;

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (uint32_t i = 0; i < paths.size(); i++)
		{
			Image image(paths[i], flipImage);

			if (!initialized)
			{
				m_Width = image.GetWidth();
				m_Height = image.GetHeight();
				int channels = image.GetChannels();
				GetChannelFormat(channels, dataFormat, internalFormat);
				glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
				initialized = true;
			}

			// creates the image on the openGL side, index is kept by object calling it.
			// must be generated in this order:
			// +X (right)
			// -X (left)
			// +Y (top)
			// -Y (bottom)
			// +Z (front)
			// -Z (back)
			// -------------------------------------------------------

			glTextureSubImage3D(m_RendererID, 0, 0, 0, i, m_Width, m_Height, 1, dataFormat, GL_UNSIGNED_BYTE, image.GetData());
		}
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTextureCube::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}