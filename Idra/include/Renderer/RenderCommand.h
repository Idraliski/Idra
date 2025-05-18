#pragma once

#include "Core/Core.h"

#include "Renderer/RendererAPI.h"

namespace Idra {
	class IDRA_API RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color) 
		{ 
			s_RendererAPI->SetClearColor(color); 
		}

		inline static void Clear() 
		{ 
			s_RendererAPI->Clear(); 
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) 
		{ 
			s_RendererAPI->DrawIndexed(vertexArray); 
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}