#pragma once

#include "Core/Core.h"

#include "Renderer/RenderCommand.h"

namespace Idra {
	class IDRA_API Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}