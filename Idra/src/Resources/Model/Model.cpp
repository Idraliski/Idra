#include "IdraPCH.h"

#include "Resources/Model/Model.h"

namespace Idra {
	Model::Model(const std::string& name)
		: m_Name(name)
	{
		IDRA_CORE_INFO("Model Constructed: {0}", name); // #DEBUG
	}

	Model::~Model()
	{
		IDRA_CORE_INFO("Model Destroyed: {0}", m_Name); // #DEBUG
	}

	void Model::AddMesh(Mesh&& mesh)
	{ 
		m_Meshes.emplace_back(std::move(mesh)); 
	}
}