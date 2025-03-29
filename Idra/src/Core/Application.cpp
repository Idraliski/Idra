#include "IdraPCH.h"
#include "Core/Application.h"

#include "Events/ApplicationEvent.h" // #DEBUG

namespace Idra {

	Application::Application()
	{
		IDRA_WARN("Application Created"); // #DEBUG
		m_Window = std::unique_ptr<Window>(Window::Create());
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

		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

} // namespace Idra