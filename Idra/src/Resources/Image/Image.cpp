#include "IdraPCH.h"

#include "Resources/Image/Image.h"

#include "stb/stb_image.h"

namespace Idra {
	Image::Image(const Path& path, bool flipImage)
	{
		// Load the image data
		stbi_set_flip_vertically_on_load(flipImage);

		m_ImageData = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);

		if (!m_ImageData)
		{
			IDRA_CORE_ERROR("Failed to load image (using default image): {0}", path.string());
			std::string defaultImage = "Assets/Textures/default.png";
			m_ImageData = stbi_load(defaultImage.c_str(), &m_Width, &m_Height, &m_Channels, 0);
			IDRA_CORE_ASSERT(m_ImageData, "Failed to load default image!");
			return;
		}

		IDRA_CORE_INFO("Loaded Image {0}", path.string());
	}

	Image::~Image()
	{
		if (m_ImageData) 
			stbi_image_free(m_ImageData);
	}
}