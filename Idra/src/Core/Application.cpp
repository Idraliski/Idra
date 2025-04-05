#include "IdraPCH.h"
#include "Core/Application.h"
#include "Core/Input.h"

#include <glad/glad.h>

namespace Idra {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		IDRA_WARN("Application Created"); // #DEBUG
		IDRA_ASSERT(!s_Instance, "Application already exists!"); // #DEBUG
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(IDRA_BIND_EVENT_FN(Application::OnEvent));
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
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(IDRA_BIND_EVENT_FN(Application::OnWindowClose));

		// Going backwards through the layer stack
		// This way, the overlay layers will get the event first 
		// and break if the event is handled
		// and then the layers will get the event last
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

} // namespace Idra