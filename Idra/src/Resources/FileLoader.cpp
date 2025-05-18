#include "IdraPCH.h"

#include "Resources/FileLoader.h"

namespace Idra {
	const std::string FileLoader::LoadFileAsString(const Path& filepath)
	{
		IDRA_CORE_ASSERT(std::filesystem::exists(filepath), "File does not exist!");
		IDRA_CORE_ASSERT(std::filesystem::is_regular_file(filepath), "Path is not a file!");

		std::ifstream file(filepath);

		// Check if the file was opened successfully
		if (!file.is_open())
		{
			IDRA_CORE_ERROR("Failed to open file: {0}", filepath.string());
			return "";
		}

		auto file_contents =
			std::string(std::istreambuf_iterator<char>(file),
				std::istreambuf_iterator<char>());

		// Check if the file is empty
		IDRA_CORE_ASSERT(!file_contents.empty(), "File is empty!");
		IDRA_CORE_INFO("File loaded successfully: {0}", filepath.string()); // #DEBUG

		return file_contents;
	}
}