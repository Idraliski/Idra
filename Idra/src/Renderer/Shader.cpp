#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Idra {
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
}