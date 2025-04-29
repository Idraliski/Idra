#include "IdraPCH.h"

#include "Renderer/VertexArray.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Idra {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IDRA_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
			case RendererAPI::API::DirectX:
				IDRA_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
				return nullptr;
			case RendererAPI::API::Vulkan:
				IDRA_ASSERT(false, "RendererAPI::Vulkan is currently not supported!");
				return nullptr;
		}

		IDRA_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}