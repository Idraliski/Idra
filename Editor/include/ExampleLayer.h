#pragma once

#include "Idra.h"

class ExampleLayer : public Idra::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	void OnUpdate() override;
	void OnEvent(Idra::Event& event) override;
};




