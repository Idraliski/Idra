#include "Renderer/Shader.h"

#include <glad/glad.h>

namespace Idra {
	Shader::Shader(const Path& vertexSrc, const Path& fragmentSrc, const std::string& name)
		: m_RendererID(0)
		, m_Name(name)
	{
		// Compile and link the shader program
		m_RendererID = glCreateProgram();
		IDRA_CORE_ASSERT(m_RendererID, "Failed to create shader program!");

		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc.string());
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc.string());

		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1f(const Path& name, float value)
	{
		int location = glGetUniformLocation(m_RendererID, name.string().c_str());
		glUniform1f(location, value);
	}

	void Shader::SetUniform1i(const Path& name, int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.string().c_str());
		glUniform1i(location, value);
	}

	void Shader::SetUniformMat4f(const Path& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.string().c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::AttachShader(const Path& src, ShaderType type)
	{
		IDRA_CORE_ASSERT(m_RendererID, "Shader program not created!");

		unsigned int glType;

		switch (type)
		{
		case ShaderType::VERTEX_SHADER:
			IDRA_CORE_INFO("Attaching vertex shader: {0}", src.string());
			glType = GL_VERTEX_SHADER;
			break;
		case ShaderType::FRAGMENT_SHADER:
			IDRA_CORE_INFO("Attaching fragment shader: {0}", src.string());
			glType = GL_FRAGMENT_SHADER;
			break;
		case ShaderType::GEOMETRY_SHADER:
			IDRA_CORE_INFO("Attaching geometry shader: {0}", src.string());
			glType = GL_GEOMETRY_SHADER;
			break;
		case ShaderType::TESS_CONTROL_SHADER:
			IDRA_CORE_INFO("Attaching tess control shader: {0}", src.string());
			glType = GL_TESS_CONTROL_SHADER;
			break;
		case ShaderType::TESS_EVAL_SHADER:
			IDRA_CORE_INFO("Attaching tess eval shader: {0}", src.string());
			glType = GL_TESS_EVALUATION_SHADER;
			break;
		case ShaderType::COMPUTE_SHADER:
			IDRA_CORE_INFO("Attaching compute shader: {0}", src.string());
			glType = GL_COMPUTE_SHADER;
			break;
		default:
			IDRA_CORE_ERROR("Unknown shader type: {0}", static_cast<int>(type));
			return;
		}

		unsigned int shader = CompileShader(glType, src.string());

		glAttachShader(m_RendererID, shader);
		glLinkProgram(m_RendererID);
		glDetachShader(m_RendererID, shader);
		glDeleteShader(shader);
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int shader = glCreateShader(type);

		// Attach the shader source code to GL
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		// Check for compilation errors
		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> message(length);
			glGetShaderInfoLog(shader, length, &length, &message[0]);

			switch (type)
			{
			case GL_VERTEX_SHADER: 
				IDRA_CORE_ERROR("Vertex shader compilation failed!"); 
				break;
			case GL_FRAGMENT_SHADER: 
				IDRA_CORE_ERROR("Fragment shader compilation failed!"); 
				break;
			case GL_GEOMETRY_SHADER:
				IDRA_CORE_ERROR("Geometry shader compilation failed!");
				break;
			case GL_COMPUTE_SHADER:
				IDRA_CORE_ERROR("Compute shader compilation failed!");
				break;
			default: 
				IDRA_CORE_ERROR("Unknown shader type, compilation failed!"); 
				break;
			}
			
			glDeleteShader(shader);

			IDRA_CORE_ERROR("{0}", message.data());
			IDRA_CORE_ASSERT(false, "Shader compilation failed!");
		}

		IDRA_CORE_INFO("Compiled {0} shader!", type == GL_VERTEX_SHADER ? "vertex" : "fragment"); // #DEBUG

		return shader;
	}
}