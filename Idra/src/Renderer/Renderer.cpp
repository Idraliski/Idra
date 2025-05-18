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

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh, const TransformComponent& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_Transform", transform.getTransformMatrix());

		// @TODO: put this into a render queue
		mesh->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model>& model, const TransformComponent& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_Transform", transform.getTransformMatrix());

		// @TODO: put this into a render queue
		for (const auto& mesh : model->GetMeshes())
		{
			mesh.Bind();
			RenderCommand::DrawIndexed(mesh.GetVertexArray());
		}
	}
}