#pragma once

#include "Core/Core.h"

#include <filesystem>

namespace Idra {
	using Path = std::filesystem::path;
	
	class IDRA_API Image
	{
	public:
		explicit Image(const Path& path, bool flipImage = true);
		~Image();

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetChannels() const { return m_Channels; }
		unsigned char* GetData() const { return m_ImageData; }

	private:
		unsigned char* m_ImageData = nullptr;

		int m_Width = 0;
		int m_Height = 0;
		int m_Channels = 0;

		bool m_FlipImage = false;
	};
}