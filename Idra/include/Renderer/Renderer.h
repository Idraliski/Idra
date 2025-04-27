#pragma once

#include "Core/Core.h"

namespace Idra {
	enum class IDRA_API RendererAPI
	{
		None = 0,
		OpenGL = 1,
		DirectX = 2,
		Vulkan = 3
	};

	class IDRA_API Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}