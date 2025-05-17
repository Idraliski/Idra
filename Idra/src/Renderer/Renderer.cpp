#include "IdraPCH.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Idra {
	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	const void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
	{
		s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);

		// @TODO: put this into a render queue
		mesh->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model>& model)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);

		// @TODO: put this into a render queue
		for (const auto& mesh : model->GetMeshes())
		{
			mesh.Bind();
			RenderCommand::DrawIndexed(mesh.GetVertexArray());
		}
	}
}