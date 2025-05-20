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

		enum class IDRA_API DepthFunction
		{
			Less = 0,
			LEqual = 1,
		};

		enum class IDRA_API CullFace
		{
			Front = 0,
			Back = 1,
			FrontAndBack = 2,
		};

	public:
		virtual void Init() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		virtual void SetDepthFunction(DepthFunction depthFunc) = 0;
		virtual void SetCullFace(CullFace cullFace) = 0;

		inline static API GetAPI() { return s_API; }
		static void SetAPI(API api) { s_API = api; }

		static Scope<RendererAPI> Create();
	private:
		// @TODO: Set this to the renderer API of your choice
		// in future we might want 1 viewport rendered in OpenGL and another in Vulkan
		static API s_API;
	};
}