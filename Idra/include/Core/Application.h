#pragma once

#include "Core/Core.h"

#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Camera.h"

namespace Idra {

	class IDRA_API Application
	{
	public:
		Application();
		~Application();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }

		void Run();

	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsMinimized = false;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	
	};

	// To be defined in the client
	Application* CreateApplication();

} // namespace Idra
