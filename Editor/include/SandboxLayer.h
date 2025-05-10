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
	std::shared_ptr<Idra::Shader> m_Shader;
	std::shared_ptr<Idra::Shader> m_FlatColourShader;
	std::shared_ptr<Idra::Mesh> m_CubeMesh;
	std::shared_ptr<Idra::Mesh> m_TriangleMesh;

	std::shared_ptr<Idra::Camera> m_Camera;
	std::shared_ptr<Idra::CameraController> m_EditorCameraController;
};




