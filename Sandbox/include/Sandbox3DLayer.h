#pragma once

#include "Idra.h"

#include <glm/glm.hpp>
#include <filesystem>

using Path = std::filesystem::path;

class Sandbox3DLayer : public Idra::Layer
{
public:
	Sandbox3DLayer();
	virtual ~Sandbox3DLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Idra::Timestep ts) override;
	void OnEvent(Idra::Event& e) override;

	void OnImGuiRender(Idra::Timestep ts) override;

private:
	void ProcessKeyInput(Idra::Timestep ts);
	void ProcessMouseInput(Idra::Timestep ts);

	bool OnKeyPressed(Idra::KeyPressedEvent& e);

private:
	float m_FPSUpdateInterval = 0.4f;
	float m_FPSUpdateCounter = 0.0f;
	float m_CurrentFPS = 0.0f;

	glm::vec3 m_Colour = { 0.2f, 0.3f, 0.8f };

	// TEMP
	// shader library
	Idra::ShaderLibrary m_ShaderLibrary;

	// Models
	Idra::Ref<Idra::Model> m_Model_Sphere;
	Idra::Ref<Idra::Model> m_Model_Cube;
	Idra::Ref<Idra::Model> m_Model_D20;
	Idra::Ref<Idra::Model> m_Model_WoodBench;
	Idra::Ref<Idra::Model> m_Model_MetalTable;
	Idra::Ref<Idra::Model> m_Model_Skybox;

	// Textures
	Idra::Ref<Idra::Texture2D> m_Texture;
	Idra::Ref<Idra::Texture2D> m_AlphaTexture;
	Idra::Ref<Idra::Texture2D> m_Texture_MetalTable;
	Idra::Ref<Idra::Texture2D> m_Texture_WoodBench;
	Idra::Ref<Idra::TextureCube> m_Texture_Skybox;

	// TEMP transforms
	Idra::TransformComponent m_Transform_Sphere;
	Idra::TransformComponent m_Transform_Sphere2;
	Idra::TransformComponent m_Transform_Cube;
	Idra::TransformComponent m_Transform_D20;
	Idra::TransformComponent m_Transform_WoodBench;
	Idra::TransformComponent m_Transform_MetalTable;

	Idra::ModelLoaderType m_ModelLoaderType = Idra::ModelLoaderType::Assimp;

	// Camera
	bool m_UsePerspectiveCamera = true;
	Idra::Ref<Idra::Camera> m_OrthoCamera;
	Idra::Ref<Idra::Camera> m_PerspectiveCamera;
	Idra::Ref<Idra::Camera> m_Camera;
	Idra::Ref<Idra::CameraController> m_EditorCameraController;
};
