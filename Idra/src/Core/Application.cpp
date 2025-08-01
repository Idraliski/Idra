#include "IdraPCH.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"

namespace Idra {

	Application* Application::s_Instance = nullptr;

	Application::Application(const WindowProps& winProps)
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_WARN("Application Created"); // #DEBUG
		IDRA_ASSERT(!s_Instance, "Application already exists!"); // #DEBUG
		s_Instance = this;

		m_Window = Window::Create(winProps);
		m_Window->SetEventCallback(IDRA_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_WARN("Application Destroyed"); // #DEBUG
	}

	void Application::Close()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_WARN("Application Close Called"); // #DEBUG

		m_IsRunning = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		IDRA_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		IDRA_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		IDRA_PROFILE_FUNCTION();

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

	void Application::Run()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_INFO("Application Running"); // #DEBUG

		while (m_IsRunning)
		{
			IDRA_PROFILE_SCOPE("Application Run Loop");

			float time = m_Window->GetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimized)
			{
				{
					IDRA_PROFILE_SCOPE("LayerStack Update");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}

				m_ImGuiLayer->Begin();
				{
					IDRA_PROFILE_SCOPE("ImGui Layer Render");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender(ts);
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		IDRA_PROFILE_FUNCTION();

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