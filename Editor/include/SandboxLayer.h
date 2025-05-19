#pragma once

#include "Idra.h"

#include <glm/glm.hpp>
#include <filesystem>

using Path = std::filesystem::path;

class SandboxLayer : public Idra::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	void OnUpdate(Idra::Timestep ts) override;
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Idra::Event& e) override;

	/*
	* If built as DLL, ImGui context is not shared across DLL boundaries.
	* Static Library builds are fine.
	* If we want to DLL, switch the MSVC to use DLL runtime library, in CMake
	*/
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
	Idra::ShaderLibrary m_ShaderLibrary;

	Idra::Ref<Idra::Model> m_Model_Sphere;
	Idra::Ref<Idra::Model> m_Model_Cube;
	Idra::Ref<Idra::Model> m_Model_D20;
	Idra::Ref<Idra::Model> m_Model_WoodBench;
	Idra::Ref<Idra::Model> m_Model_MetalTable;

	Idra::Ref<Idra::Texture2D> m_Texture;
	Idra::Ref<Idra::Texture2D> m_AlphaTexture;
	Idra::Ref<Idra::Texture2D> m_Texture_MetalTable;
	Idra::Ref<Idra::Texture2D> m_Texture_WoodBench;

	// TEMP transforms
	Idra::TransformComponent m_Transform_Sphere;
	Idra::TransformComponent m_Transform_Sphere2;
	Idra::TransformComponent m_Transform_Cube;
	Idra::TransformComponent m_Transform_D20;
	Idra::TransformComponent m_Transform_WoodBench;
	Idra::TransformComponent m_Transform_MetalTable;

	Idra::ModelLoaderType m_ModelLoaderType = Idra::ModelLoaderType::Assimp;

	Idra::Ref<Idra::Camera> m_Camera;
	Idra::Ref<Idra::CameraController> m_EditorCameraController;
};




