#include "IdraPCH.h"
#include "Core/Application.h"

#include <glad/glad.h>

namespace Idra {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1) // #DEBUG

	Application::Application()
	{
		IDRA_WARN("Application Created"); // #DEBUG
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
		IDRA_WARN("Application Destroyed"); // #DEBUG
	}

	void Application::Run()
	{
		IDRA_INFO("Application Running"); // #DEBUG

		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		if(e.GetEventType() != EventType::MouseMoved) // #DEBUG
			IDRA_TRACE("{0}", e); // #DEBUG
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

} // namespace Idra