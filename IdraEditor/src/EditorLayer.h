#pragma once

#include "Idra.h"

#include <glm/glm.hpp>

namespace Idra {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		void OnImGuiRender(Timestep ts) override;

	private:
		void ProcessKeyInput(Timestep ts);
		void ProcessMouseInput(Timestep ts);

		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		float m_FPSUpdateInterval = 0.4f;
		float m_FPSUpdateCounter = 0.0f;
		float m_CurrentFPS = 0.0f;

		glm::vec3 m_Colour = { 0.2f, 0.3f, 0.8f };

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		// TEMP
		// Frame Buffer
		Ref<FrameBuffer> m_FrameBuffer;

		// shader library
		ShaderLibrary m_ShaderLibrary;

		// Models
		Ref<Model> m_Model_Sphere;
		Ref<Model> m_Model_Cube;
		Ref<Model> m_Model_D20;
		Ref<Model> m_Model_WoodBench;
		Ref<Model> m_Model_MetalTable;
		Ref<Model> m_Model_Skybox;

		// Textures
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_AlphaTexture;
		Ref<Texture2D> m_Texture_MetalTable;
		Ref<Texture2D> m_Texture_WoodBench;
		Ref<TextureCube> m_Texture_Skybox;

		// TEMP transforms
		TransformComponent m_Transform_Sphere;
		TransformComponent m_Transform_Sphere2;
		TransformComponent m_Transform_Cube;
		TransformComponent m_Transform_D20;
		TransformComponent m_Transform_WoodBench;
		TransformComponent m_Transform_MetalTable;

		ModelLoaderType m_ModelLoaderType = ModelLoaderType::Assimp;

		// Camera
		bool m_UsePerspectiveCamera = true;
		Ref<Camera> m_OrthoCamera;
		Ref<Camera> m_PerspectiveCamera;
		Ref<Camera> m_Camera;
		Ref<CameraController> m_EditorCameraController;
	};

}
