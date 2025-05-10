#pragma once

#include "Core/Core.h"

#include <filesystem>
#include <string>

namespace Idra {
	using Path = ::std::filesystem::path;

	class IDRA_API FileLoader
	{
	public:
		static const std::string LoadFileAsString(const Path& filepath);
	};
}
