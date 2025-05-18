#pragma once

#include "Core/Core.h"

#include "Renderer/RendererAPI.h"

namespace Idra {
	class IDRA_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}