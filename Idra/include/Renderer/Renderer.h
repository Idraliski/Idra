#pragma once

#include "Core/Core.h"

#include "Renderer/Camera.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Resources/Model/Mesh.h"
#include "Resources/Model/Model.h"
#include "Scene/Components.h"

#include <glm/glm.hpp>

namespace Idra {
	class IDRA_API Renderer
	{
	public:
		static void Init();

		static const void BeginScene(const Ref<Camera>& camera, const Ref<Shader>& skyboxShader, const Ref<Model>& skyboxModel);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const TransformComponent& transform);
		static void Submit(const Ref<Shader>& shader, const Ref<Model>& model, const TransformComponent& transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static void DrawSkybox(const Ref<Shader>& shader, const Ref<Model>& model);

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
		};

		static SceneData s_SceneData;
	};
}