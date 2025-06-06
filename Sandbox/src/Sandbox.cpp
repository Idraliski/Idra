#include "Idra.h"
#include "Core/EntryPoint.h"

#include "SandboxLayer.h"
#include "Sandbox3DLayer.h"

class Sandbox : public Idra::Application
{
public:
	Sandbox()
		: Application(Idra::WindowProps{ "Sandbox", 1280, 720 })
	{
		IDRA_WARN("Sandbox Created"); // #DEBUG
		//PushLayer(new SandboxLayer());
		PushLayer(new Sandbox3DLayer());
	}

	~Sandbox()
	{
		IDRA_WARN("Sandbox Destroyed"); // #DEBUG
	}
};

Idra::Application* Idra::CreateApplication()
{
	return new Sandbox();
}