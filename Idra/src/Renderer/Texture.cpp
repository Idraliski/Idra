#include "IdraPCH.h"

#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Idra {
	Texture2D* Texture2D::Create(const Path& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLTexture2D(path);

		case RendererAPI::API::DirectX:
			IDRA_CORE_ASSERT(false, "RendererAPI::DirectX is not supported!");
			return nullptr;

		case RendererAPI::API::Vulkan:
			IDRA_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!");
			return nullptr;
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}