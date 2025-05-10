#pragma once

#include "Idra.h"

#include <glm/glm.hpp>

#include <filesystem>

using Path = std::filesystem::path;

class ExampleLayer : public Idra::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	void OnUpdate(Idra::Timestep ts) override;
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Idra::Event& e) override;

	void OnImGuiRender(Idra::Timestep ts) override;

private:
	void ProcessKeyInput(Idra::Timestep ts);
	void ProcessMouseInput(Idra::Timestep ts);

private:
	float m_FPSUpdateInterval = 0.4f;
	float m_FPSUpdateCounter = 0.0f;
	float m_CurrentFPS = 0.0f;

	glm::vec3 m_Colour = { 0.2f, 0.3f, 0.8f };

	// TEMP
	std::shared_ptr<Idra::Shader> m_Shader;
	std::shared_ptr<Idra::Shader> m_FlatColourShader;
	std::shared_ptr<Idra::VertexArray> m_VertexArray;
	std::shared_ptr<Idra::VertexArray> m_TriVA;

	std::shared_ptr<Idra::Camera> m_Camera;
	std::shared_ptr<Idra::CameraController> m_EditorCameraController;
};




