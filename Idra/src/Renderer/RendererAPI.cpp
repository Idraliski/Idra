#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Idra {
	// @TODO: Set this to the renderer API of your choice
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
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