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

		static const void BeginScene(const std::shared_ptr<Camera>& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh, const TransformComponent& transform);
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model>& model, const TransformComponent& transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData s_SceneData;
	};
}