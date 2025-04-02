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
	//IDRA_INFO("ExampleLayer::Update");
}

void ExampleLayer::OnEvent(Idra::Event& event) 
{
	IDRA_TRACE("{0}", event);
}