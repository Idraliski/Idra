#include "IdraPCH.h"

#include "Renderer/Renderer.h"

namespace Idra {
	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::Init()
	{
		IDRA_PROFILE_FUNCTION();

		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		IDRA_PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

	const void Renderer::BeginScene(const Ref<Camera>& camera, const Ref<Shader>& skyboxShader, const Ref<Model>& skyboxModel)
	{
		IDRA_PROFILE_FUNCTION();

		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();

		s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_SceneData.ProjectionMatrix = camera->GetProjectionMatrix();
		s_SceneData.ViewMatrix = camera->GetViewMatrix();
		if (camera->GetProjectionType() == CameraProjectionType::Perspective)
			s_SceneData.PerspectiveProjectionMatrix = camera->GetProjectionMatrix();

		if (skyboxShader && skyboxModel)
			DrawSkybox(skyboxShader, skyboxModel);

	}

	void Renderer::EndScene()
	{
		IDRA_PROFILE_FUNCTION();

		// Future render queue implementation

		// RenderCommand::Flush();
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::Less);
		RenderCommand::SetCullFace(RendererAPI::CullFace::Back);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const TransformComponent& transform)
	{
		IDRA_PROFILE_FUNCTION();

		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		shader->SetUniformMat4f("u_Transform", transform.getTransformMatrix());

		// @TODO: put this into a render queue
		mesh->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Model>& model, const TransformComponent& transform)
	{
		IDRA_PROFILE_FUNCTION();

		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		shader->SetUniformMat4f("u_Transform", transform.getTransformMatrix());

		// @TODO: put this into a render queue
		for (const auto& mesh : model->GetMeshes())
		{
			mesh.Bind();
			RenderCommand::DrawIndexed(mesh.GetVertexArray());
		}
	}

	void Renderer::DrawSkybox(const Ref<Shader>& shader, const Ref<Model>& model)
	{
		IDRA_PROFILE_FUNCTION();

		//remove translation from view matrix
		glm::mat4 view = glm::mat4(glm::mat3(s_SceneData.ViewMatrix));
		glm::mat4 projection = s_SceneData.PerspectiveProjectionMatrix;
		glm::mat4 viewProjection = projection * view;

		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", viewProjection);

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