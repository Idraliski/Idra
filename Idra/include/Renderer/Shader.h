#pragma once

#include "Core/Core.h"

#include <filesystem>
#include <string>

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
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;

		static Ref<Shader> Create(const Path& src);
		static Ref<Shader> Create(const Path& vertexSrc, const Path& fragmentSrc);
	};

	class IDRA_API ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const Path& path);
		Ref<Shader> Load(const std::string& name, const Path& path);
		Ref<Shader> Load(const std::string& name, const Path& vertexSrc, const Path& fragmentSrc);

		Ref<Shader> Get(const std::string& name);

		bool DoesExists(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}