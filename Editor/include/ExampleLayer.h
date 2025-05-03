#pragma once

#include "Idra.h"

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

private:
	float m_FPSUpdateInterval = 0.4f;
	float m_FPSUpdateCounter = 0.0f;
	float m_CurrentFPS = 0.0f;

	// TEMP
	std::shared_ptr<Idra::Shader> m_Shader;
	std::shared_ptr<Idra::Shader> m_BlueShader;
	std::shared_ptr<Idra::VertexArray> m_VertexArray;
	std::shared_ptr<Idra::VertexArray> m_TriVA;

	std::unique_ptr<Idra::Camera> m_Camera;
};




