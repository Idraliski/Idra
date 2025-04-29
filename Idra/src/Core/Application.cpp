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

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// TEMP DRAW DATA
		m_VertexArray.reset(VertexArray::Create());

		// Vertex data: position (x, y, z), colour (r, g, b, a)
		float vertices[4 * 7] = {
			-0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.75f, -0.75f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.75f,  0.75f, 0.0f, 0.2f, 0.8f, 0.5f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.6f, 0.4f, 0.4f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout bufferlayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		squareVB->SetLayout(bufferlayout);
		m_VertexArray->AddVertexBuffer(squareVB);

		uint32_t indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(squareIB);

		m_TriVA.reset(VertexArray::Create());
		float triVertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};
		std::shared_ptr<VertexBuffer> triVB;
		triVB.reset(VertexBuffer::Create(triVertices, sizeof(triVertices)));
		triVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_TriVA->AddVertexBuffer(triVB);

		uint32_t triIndices[3] = {
			0, 1, 2
		};
		std::shared_ptr<IndexBuffer> triIB;
		triIB.reset(IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));
		m_TriVA->SetIndexBuffer(triIB);

		// TEMP
		// Load and compile the vertex shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;
			out vec3 v_Position;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Color = a_Color;
				v_Position = a_Position;
			}
		)";

		// Load and compile the fragment shader
		// This is a simple shader that just outputs the texture color
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_Color;
			in vec3 v_Position;

			void main()
			{
				color = v_Color;
				//color = vec4(v_Position * 0.5 + 0.5, 1.0); //static color based on position
				//color = vec4(0.8, 0.2, 0.3, 1.0); //static uniform color

			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc, "BasicShader");

		// TEMP
		// Load and compile the vertex shader
		std::string blueVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Position = a_Position;
			}
		)";

		// Load and compile the fragment shader
		// This is a simple shader that just outputs the texture color
		std::string blueFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_Color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0); //static uniform color
			}
		)";

		m_BlueShader = std::make_unique<Shader>(blueVertexSrc, blueFragmentSrc, "BasicBlueShader");
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

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

			m_BlueShader->Bind();
			m_TriVA->Bind();
			glDrawElements(GL_TRIANGLES, m_TriVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
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