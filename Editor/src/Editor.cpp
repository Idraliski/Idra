#include "Idra.h"
#include "Core/EntryPoint.h"

#include "ExampleLayer.h"

class Editor : public Idra::Application
{
public:
	Editor() 
	{
		IDRA_INFO("Editor Created"); // #DEBUG
		PushLayer(new ExampleLayer());
		PushOverlay(new Idra::ImGuiLayer());
	}

	~Editor() 
	{
		IDRA_INFO("Editor Destroyed"); // #DEBUG
	}
};

Idra::Application* Idra::CreateApplication()
{
	return new Editor();
}