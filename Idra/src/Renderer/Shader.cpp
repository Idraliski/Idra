#include "IdraPCH.h"

#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Idra {
	Ref<Shader> Shader::Create(const Path& src)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLShader>(src);
			case RendererAPI::API::Vulkan:
				IDRA_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!");
				return nullptr;
			case RendererAPI::API::DirectX:
				IDRA_CORE_ASSERT(false, "RendererAPI::DirectX is not supported!");
				return nullptr;
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const Path& vertexSrc, const Path& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!"); 
				return nullptr;
			case RendererAPI::API::OpenGL: 
				return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
			case RendererAPI::API::Vulkan: 
				IDRA_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!"); 
				return nullptr;
			case RendererAPI::API::DirectX: 
				IDRA_CORE_ASSERT(false, "RendererAPI::DirectX is not supported!"); 
				return nullptr;
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const Path& path)
	{
		if (DoesExists(path.stem().string()))
			return Get(path.stem().string());

		auto shader = Shader::Create(path);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const Path& path)
	{
		if (DoesExists(name))
			return Get(name);

		auto shader = Shader::Create(path);
		shader->SetName(name);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, 
		const Path& vertexSrc, const Path& fragmentSrc)
	{
		if (DoesExists(name))
			return Get(name);

		auto shader = Shader::Create(vertexSrc, fragmentSrc);
		shader->SetName(name);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		IDRA_CORE_ASSERT(DoesExists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::DoesExists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}