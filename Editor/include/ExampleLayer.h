#pragma once

#include "Idra.h"

class ExampleLayer : public Idra::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	void OnUpdate() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Idra::Event& e) override;

	void OnImGuiRender() override;
private:
	bool OnKeyPressed(Idra::KeyPressedEvent& e);
private:
	// TEMP
	std::shared_ptr<Idra::Shader> m_Shader;
	std::shared_ptr<Idra::Shader> m_BlueShader;
	std::shared_ptr<Idra::VertexArray> m_VertexArray;
	std::shared_ptr<Idra::VertexArray> m_TriVA;

	std::unique_ptr<Idra::Camera> m_Camera;
};




