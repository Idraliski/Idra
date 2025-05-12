#pragma once

#include "Core/Core.h"

#include "Model/Mesh.h"

namespace Idra {
	using Path = std::filesystem::path;

	class IDRA_API Model
	{
	public:
		Model(const std::string& name);
		Model(const std::string& name, const std::vector<Mesh> meshes);
		~Model();

		inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		void AddMesh(Mesh&& mesh);

	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Name;
	};
}