#pragma once

#include "Core/Core.h"

#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace Idra {
	class IDRA_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const Path& src);
		OpenGLShader(const Path& vertexSrc, const Path& fragmentSrc);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, const glm::vec2& value);
		void SetUniform3f(const std::string& name, const glm::vec3& value);
		void SetUniform4f(const std::string& name, const glm::vec4& value);

		void SetUniform1i(const std::string& name, int value);

		void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

		void AttachShader(const Path& src, ShaderType type);

	private:
		std::unordered_map<unsigned int, std::string> ParseShader(const std::string& src);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID = 0;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}