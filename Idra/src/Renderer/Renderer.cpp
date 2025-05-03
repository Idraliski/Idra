#include "IdraPCH.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Idra {
	Renderer::SceneData Renderer::s_SceneData;

	const void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
	{
		s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);

		// @TODO: put this into a render queue
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}