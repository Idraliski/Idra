#include "Idra.h"
#include "Core/EntryPoint.h"

#include "ExampleLayer.h"

class Editor : public Idra::Application
{
public:
	Editor() 
	{
		PushLayer(new ExampleLayer());
	}

	~Editor() 
	{
	}
};

Idra::Application* Idra::CreateApplication()
{
	return new Editor();
}