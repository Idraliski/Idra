#include "IdraPCH.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Idra {
	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	const void Renderer::BeginScene(const Ref<Camera>& camera, const Ref<Shader>& skyboxShader, const Ref<Model>& skyboxModel)
	{
		s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_SceneData.ProjectionMatrix = camera->GetProjectionMatrix();
		s_SceneData.ViewMatrix = camera->GetViewMatrix();

		if (skyboxShader && skyboxModel)
			DrawSkybox(skyboxShader, skyboxModel);
	}

	void Renderer::EndScene()
	{
		// Future render queue implementation

		// RenderCommand::Flush();
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::Less);
		RenderCommand::SetCullFace(RendererAPI::CullFace::Back);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const TransformComponent& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_Transform", transform.getTransformMatrix());

		// @TODO: put this into a render queue
		mesh->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Model>& model, const TransformComponent& transform)
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

	void Renderer::DrawSkybox(const Ref<Shader>& shader, const Ref<Model>& model)
	{
		//remove translation from view matrix
		glm::mat4 view = glm::mat4(glm::mat3(s_SceneData.ViewMatrix));
		glm::mat4 projection = s_SceneData.ProjectionMatrix;
		glm::mat4 viewProjection = projection * view;

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", viewProjection);

		//  change depth function so depth test passes when
		//  values are equal to depth buffer's content
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LEqual);
		// set cull face to front so we can see the inside of the skybox
		RenderCommand::SetCullFace(RendererAPI::CullFace::Front);

		for (const auto& mesh : model->GetMeshes())
		{
			mesh.Bind();
			RenderCommand::DrawIndexed(mesh.GetVertexArray());
		}

		//  reset depth function to default
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::Less);
		// set cull face to back to default for the rest of program
		RenderCommand::SetCullFace(RendererAPI::CullFace::Back);
	}
}