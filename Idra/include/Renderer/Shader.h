#pragma once

#include "IdraPCH.h"

#include "Core/Core.h"
#include "glm/glm.hpp"

namespace Idra {
	using Path = ::std::filesystem::path;

	enum class ShaderType
	{
		VERTEX_SHADER = 0,
		FRAGMENT_SHADER = 1,
		GEOMETRY_SHADER = 2,
		TESS_CONTROL_SHADER = 3,
		TESS_EVAL_SHADER = 4,
		COMPUTE_SHADER = 5
	};

	class IDRA_API Shader
	{
	public:
		Shader(const Path& vertexSrc, const Path& fragmentSrc, const std::string& name);
		~Shader();

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }
		uint32_t GetRendererID() const { return m_RendererID; }

		void Bind() const;
		void Unbind() const;

		void SetUniform1f(const Path& name, float value);
		void SetUniform1i(const Path& name, int value);
		void SetUniformMat4f(const Path& name, const glm::mat4& matrix);

		void AttachShader(const Path& src, ShaderType type);
	private:
		unsigned int CompileShader(unsigned int type, const std::string& source);

		uint32_t m_RendererID;
		std::string m_Name;
	};
}