#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate() 
{
	//IDRA_INFO("ExampleLayer::Update"); // #DEBUG
}

void ExampleLayer::OnEvent(Idra::Event& e) 
{
	if (e.GetEventType() != Idra::EventType::MouseMoved)
		IDRA_TRACE("Example Layer: {0}", e); // #DEBUG
}