#pragma once

#include "Idra.h"

class Editor : public Idra::Application
{
public:
	Editor()
	{
	}

	~Editor()
	{
	}

};

Idra::Application* Idra::CreateApplication()
{
	return new Editor();
}
