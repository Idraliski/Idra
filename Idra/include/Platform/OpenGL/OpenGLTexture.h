#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace Idra {
	static void GetChannelFormat(int channels, unsigned int& dataFormat, unsigned int& internalFormat);

	class IDRA_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const Path& path, bool flipImage = true);
		~OpenGLTexture2D();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

	private:
		Path m_Path;
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
	};

	class IDRA_API OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const std::vector<Path>& path, bool flipImage = true);
		~OpenGLTextureCube();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
	private:
		std::vector<Path> m_Paths;
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
	};
}