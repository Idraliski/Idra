#include "IdraPCH.h"

#include "Resources/Model/ModelLoader.h"

namespace Idra {
	Ref<Model> ModelLoader::LoadModel(ModelLoaderType type, Path source)
	{
		switch (type)
		{
			case ModelLoaderType::Assimp:
			{
				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFile(source.string(), 
					aiProcess_Triangulate | 
					aiProcess_JoinIdenticalVertices |
					aiProcess_ValidateDataStructure |
					aiProcess_GenSmoothNormals |
					aiProcess_CalcTangentSpace |
					aiProcess_FlipUVs
				);

				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
					IDRA_CORE_ERROR("ASSIMP ERROR: {0}", importer.GetErrorString());
					return nullptr;
				}

				Ref<Model> model = CreateRef<Model>(source.stem().string());
				ProcessAssimpNode(scene->mRootNode, scene, model);
				return model;
			}
			case ModelLoaderType::MD2:
				IDRA_CORE_ERROR("ModelLoader: MD2 model loader not implemented yet!");
				return nullptr;
			default:
				IDRA_CORE_ERROR("ModelLoader: Unknown model loader type!");
				return nullptr;
		}
	}

	std::string ModelLoader::ModelLoaderTypeToString(ModelLoaderType type)
	{
		switch (type)
		{
			case ModelLoaderType::Assimp: return "Assimp";
			case ModelLoaderType::MD2: return "MD2";
			default: return "Unknown";
		}
	}

	void ModelLoader::ProcessAssimpNode(aiNode* node, const aiScene* scene, Ref<Model> model)
	{
		// Process all the meshes in this node
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model->AddMesh(ProcessAssimpMesh(mesh, scene));
		}

		// Then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessAssimpNode(node->mChildren[i], scene, model);
	}

	Mesh ModelLoader::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene)
	{
		IDRA_CORE_ASSERT(mesh->HasPositions(), "Mesh has no positions!"); // #DEBUG

		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		// Determine buffer layout format
		BufferLayout::VertexFormat format = BufferLayout::VertexFormat::Pos;
		if (mesh->HasNormals())
			format = BufferLayout::VertexFormat::PosNor;
		else if (mesh->HasTextureCoords(0))
			format = BufferLayout::VertexFormat::PosTex;
		if (mesh->HasNormals() && mesh->HasTextureCoords(0))
			format = BufferLayout::VertexFormat::PosNorTex;
		BufferLayout layout = BufferLayout::GetVertexFormat(format);
		
		// Process vertices
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D pos = mesh->mVertices[i];
			vertices.push_back(pos.x);
			vertices.push_back(pos.y);
			vertices.push_back(pos.z);

			if (mesh->HasNormals())
			{
				aiVector3D normal = mesh->mNormals[i];
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}

			if (mesh->HasTextureCoords(0))
			{
				aiVector3D texCoord = mesh->mTextureCoords[0][i];
				vertices.push_back(texCoord.x);
				vertices.push_back(texCoord.y);
			}
		}

		// Process indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh(vertices, layout, indices);
	}
}