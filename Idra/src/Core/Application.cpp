#include "IdraPCH.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"

namespace Idra {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		IDRA_WARN("Application Created"); // #DEBUG
		IDRA_ASSERT(!s_Instance, "Application already exists!"); // #DEBUG
		s_Instance = this;

		m_Window = CreateScope<Window>(Window::Create());
		m_Window->SetEventCallback(IDRA_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		IDRA_WARN("Application Destroyed"); // #DEBUG
	}

	void Application::Run()
	{
		IDRA_INFO("Application Running"); // #DEBUG

		while (m_IsRunning)
		{
			float time = m_Window->GetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(ts);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(ts);
			m_ImGuiLayer->End();

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
		dispatcher.Dispatch<WindowResizeEvent>(IDRA_BIND_EVENT_FN(Application::OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_IsMinimized = true;
			return false;
		}

		m_IsMinimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

} // namespace Idra