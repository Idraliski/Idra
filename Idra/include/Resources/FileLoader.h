#pragma once

#include "Core/Core.h"
#include "Resources/Image/Image.h"

#include <filesystem>
#include <string>

namespace Idra {
	using Path = ::std::filesystem::path;

	class IDRA_API FileLoader
	{
	public:
		static const std::string LoadFileAsString(const Path& filepath);
		static Ref<Image> LoadFileAsImage(const Path& filepath, bool flipImage = true);
	};
}
