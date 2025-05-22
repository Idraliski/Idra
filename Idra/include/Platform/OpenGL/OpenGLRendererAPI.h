#pragma once

#include "Core/Core.h"

#include "Renderer/RendererAPI.h"

namespace Idra {
	class IDRA_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void SetDepthFunction(DepthFunction depthFunc) override;
		void SetCullFace(CullFace cullFace) override;
	};
}