#pragma once

#include "Core/Core.h"

#include "Resources/Model/Model.h"
#include "Resources/Model/Mesh.h"

#include <vector>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Idra {
	using Path = std::filesystem::path;

	enum class IDRA_API ModelLoaderType
	{
		Assimp = 0,
		MD2 = 1
	};

	class IDRA_API ModelLoader
	{
	public:
		static Ref<Model> LoadModel(ModelLoaderType type, Path source);

		static std::string ModelLoaderTypeToString(ModelLoaderType type);
	private:
		static void ProcessAssimpNode(aiNode* node, const aiScene* scene, Ref<Model> model);
		static Mesh ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene);
	};
}