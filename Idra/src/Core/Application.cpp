#include "IdraPCH.h"
#include "Core/Application.h"

#include "Events/ApplicationEvent.h" // #DEBUG
#include "Core/Log.h" // #DEBUG

namespace Idra {

	Application::Application()
	{
		IDRA_WARN("Application Created"); // #DEBUG
	}

	Application::~Application()
	{
		IDRA_WARN("Application Destroyed"); // #DEBUG
	}

	void Application::Run()
	{
		IDRA_INFO("Application Running"); // #DEBUG
		WindowResizeEvent e(1280, 720); // #DEBUG
		IDRA_TRACE(e); // #DEBUG

		while (true);
	}

} // namespace Idra