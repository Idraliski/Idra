#include "Idra.h"
#include "Core/EntryPoint.h"

#include "SandboxLayer.h"

class Editor : public Idra::Application
{
public:
	Editor() 
	{
		IDRA_INFO("Editor Created"); // #DEBUG
		PushLayer(new SandboxLayer());
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