#pragma once

#include "Core/Core.h"
#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Idra {
	class IDRA_API RendererAPI
	{
	public:
		enum class IDRA_API API
		{
			None = 0,
			OpenGL = 1,
			DirectX = 2,
			Vulkan = 3
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
		static void SetAPI(API api) { s_API = api; }

		static RendererAPI* Create();
	private:
		static API s_API;
	};
}