#include "IdraPCH.h"

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Idra {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None: 
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::OpenGL: 
				return new OpenGLVertexBuffer(vertices, size);
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None: 
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!"); 
				return nullptr;
			case RendererAPI::OpenGL: 
				return new OpenGLIndexBuffer(indices, count);
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}