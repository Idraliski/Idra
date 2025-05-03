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

		static Shader* Create(const Path& vertexSrc, const Path& fragmentSrc);		
	};
}