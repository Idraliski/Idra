#include "Idra.h"
#include "Core/EntryPoint.h"

#include "SandboxLayer.h"
#include "Sandbox3DLayer.h"

class SandboxEditor : public Idra::Application
{
public:
	SandboxEditor()
		: Application(Idra::WindowProps{ "Sandbox Editor", 1280, 720 })
	{
		IDRA_WARN("Sandbox Editor Created"); // #DEBUG
		//PushLayer(new SandboxLayer());
		PushLayer(new Sandbox3DLayer());
	}

	~SandboxEditor()
	{
		IDRA_WARN("Sandbox Editor Destroyed"); // #DEBUG
	}
};

Idra::Application* Idra::CreateApplication()
{
	return new SandboxEditor();
}