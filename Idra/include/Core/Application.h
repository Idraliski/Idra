#pragma once

#include "Core/Core.h"

#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "Core/LayerStack.h"

#include "ImGui/ImGuiLayer.h"

//TEMP
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

namespace Idra {

	class IDRA_API Application
	{
	public:
		Application();
		~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		// TEMP
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexArray> m_TriVA;
	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();

} // namespace Idra
