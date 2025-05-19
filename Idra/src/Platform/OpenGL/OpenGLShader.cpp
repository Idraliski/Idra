#include "IdraPCH.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Resources/FileLoader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Idra {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		else if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		else if (type == "tess_control")
			return GL_TESS_CONTROL_SHADER;
		else if (type == "tess_evaluation")
			return GL_TESS_EVALUATION_SHADER;
		else if (type == "compute")
			return GL_COMPUTE_SHADER;

		IDRA_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const Path& src)
		: m_RendererID(0), m_Name(src.stem().string())
	{
		// Compile and link the shader program
		m_RendererID = glCreateProgram();
		IDRA_CORE_ASSERT(m_RendererID, "Failed to create shader program!");

		// Load the shader source code from file
		std::string shaderSrc = FileLoader::LoadFileAsString(src);
		auto shaderSources = ParseShader(shaderSrc);
		IDRA_CORE_ASSERT(shaderSources.size() > 0, "No shader sources found!");
		IDRA_CORE_ASSERT(shaderSources.find(GL_VERTEX_SHADER) != shaderSources.end(), "Vertex shader not found!");
		IDRA_CORE_ASSERT(shaderSources.find(GL_FRAGMENT_SHADER) != shaderSources.end(), "Fragment shader not found!");

		std::vector<GLuint> shaders;

		for (auto& kv : shaderSources)
		{
			GLuint shader = CompileShader(kv.first, kv.second);
			shaders.push_back(shader);
		}

		glLinkProgram(m_RendererID);

		for (auto& shader : shaders)
		{
			glDetachShader(m_RendererID, shader);
			glDeleteShader(shader);
		}
	}

	OpenGLShader::OpenGLShader(const Path& vertexSrc, const Path& fragmentSrc)
		: m_RendererID(0), m_Name(vertexSrc.stem().string())
	{
		// Compile and link the shader program
		m_RendererID = glCreateProgram();
		IDRA_CORE_ASSERT(m_RendererID, "Failed to create shader program!");

		// Load the shader source code from file
		std::string vertexSrcStr = FileLoader::LoadFileAsString(vertexSrc);
		std::string fragmentSrcStr = FileLoader::LoadFileAsString(fragmentSrc);

		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrcStr);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrcStr);

		glLinkProgram(m_RendererID);

		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::AttachShader(const Path& src, ShaderType type)
	{
		IDRA_CORE_ASSERT(m_RendererID, "Shader program not created!");

		GLuint glType;

		// @TODO: check to see if vertex and fragment shaders are already attached
		// maybe override it or just ignore it

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

		// Load the shader source code from file
		std::string shaderSrc = FileLoader::LoadFileAsString(src);

		GLuint shader = CompileShader(glType, shaderSrc);

		glLinkProgram(m_RendererID);
		glDetachShader(m_RendererID, shader);
		glDeleteShader(shader);
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::ParseShader(const std::string& src)
	{
		std::unordered_map<unsigned int, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);
		IDRA_CORE_ASSERT(pos != std::string::npos, "Cannot Find #type");

		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			IDRA_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = src.substr(begin, eol - begin);
			IDRA_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = 
				(pos == std::string::npos) 
					? src.substr(nextLinePos) 
					: src.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);

		// Attach the shader source code to GL
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		// Check for compilation errors
		GLint result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length = 0;
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

		glAttachShader(m_RendererID, shader);
		return shader;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		// Check if the uniform location is already cached
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		// If not, retrieve the location from OpenGL, and cache it
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			IDRA_CORE_WARN("Uniform '{0}' not found!", name);
		m_UniformLocationCache[name] = location;

		return location;
	}
}