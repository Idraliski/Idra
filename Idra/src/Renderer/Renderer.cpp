#include "Renderer/Renderer.h"

namespace Idra {

	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		// @TODO: put this into a render queue
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}