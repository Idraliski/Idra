#include "IdraPCH.h"

#include "Resources/Model/Model.h"

namespace Idra {
	Model::Model(const std::string& name)
		: m_Name(name)
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_CORE_INFO("Model Constructed: {0}", name); // #DEBUG
	}

	Model::~Model()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_CORE_INFO("Model Destroyed: {0}", m_Name); // #DEBUG
	}

	void Model::AddMesh(Mesh&& mesh)
	{
		IDRA_PROFILE_FUNCTION();

		m_Meshes.emplace_back(std::move(mesh)); 
	}
}