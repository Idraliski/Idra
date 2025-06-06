#include "IdraPCH.h"

#include "Renderer/Renderer.h"
#include "Renderer/FrameBuffer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Idra {
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IDRA_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLFrameBuffer>(spec);
			case RendererAPI::API::Vulkan:
				IDRA_ASSERT(false, "RendererAPI::Vulkan is not supported yet!");
				return nullptr;
			case RendererAPI::API::DirectX:
				IDRA_ASSERT(false, "RendererAPI::DirectX is not supported yet!");
				return nullptr;
		}

		IDRA_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}