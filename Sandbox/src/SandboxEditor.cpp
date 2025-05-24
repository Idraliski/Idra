#include "Idra.h"
#include "Core/EntryPoint.h"

#include "SandboxLayer.h"
#include "Sandbox3DLayer.h"

class SandboxEditor : public Idra::Application
{
public:
	SandboxEditor()
	{
		IDRA_INFO("Editor Created"); // #DEBUG
		//PushLayer(new SandboxLayer());
		PushLayer(new Sandbox3DLayer());
	}

	~SandboxEditor()
	{
		IDRA_INFO("Editor Destroyed"); // #DEBUG
	}
};

Idra::Application* Idra::CreateApplication()
{
	return new SandboxEditor();
}