#include "IdraPCH.h"

#include "Resources/FileLoader.h"

namespace Idra {
	const std::string FileLoader::LoadFileAsString(const Path& filepath)
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_CORE_ASSERT(std::filesystem::exists(filepath), "File does not exist!");
		IDRA_CORE_ASSERT(std::filesystem::is_regular_file(filepath), "Path is not a file!");

		std::ifstream file(filepath, std::ios::in | std::ios::binary);

		// Check if the file was opened successfully
		if (!file.is_open())
		{
			IDRA_CORE_ERROR("Failed to open file: {0}", filepath.string());
			return "";
		}

		file.seekg(0, std::ios::end); // Move to the end of the file
		size_t fileSize = file.tellg(); // Get the current position in the file
		IDRA_CORE_ASSERT(fileSize != static_cast<size_t>(-1), "Failed to get file size!"); // Check if the file size is valid

		std::string fileContents(fileSize, '\0'); // Create a string with the size of the file
		file.seekg(0, std::ios::beg); // Move back to the beginning of the file
		file.read(&fileContents[0], fileSize); // Read the file contents into the string

		// Check if the file is empty
		IDRA_CORE_ASSERT(fileSize > 0, "File is empty!"); // Check if the file is empty
		IDRA_CORE_INFO("File loaded successfully: {0}", filepath.string()); // #DEBUG

		return fileContents;
	}
}