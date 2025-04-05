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
};




