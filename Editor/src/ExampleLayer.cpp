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

	if (Idra::Input::IsKeyPressed(IDRA_KEY_TAB))
		IDRA_TRACE("Tab key is pressed! (Poll!)"); // #DEBUG
}
void ExampleLayer::OnAttach()
{
	IDRA_INFO("Example Layer Attached"); // #DEBUG
}

void ExampleLayer::OnDetach()
{
	IDRA_INFO("Example Layer Detached"); // #DEBUG
}

void ExampleLayer::OnEvent(Idra::Event& e) 
{
	if (e.GetEventType() != Idra::EventType::MouseMoved)
		IDRA_TRACE("Example Layer: {0}", e); // #DEBUG

	if (e.GetEventType() == Idra::EventType::KeyPressed)
	{
		Idra::KeyPressedEvent& keyEvent = (Idra::KeyPressedEvent&)e;
		IDRA_TRACE("Key Pressed: {0}", (char)keyEvent.GetKeyCode()); // #DEBUG
	}
}