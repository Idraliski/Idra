#include "IdraPCH.h"

#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Idra {
	Ref<Texture2D> Texture2D::Create(const Path& path, bool flipImage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path, flipImage);

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