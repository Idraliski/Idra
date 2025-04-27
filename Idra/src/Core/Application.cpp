#include "IdraPCH.h"
#include "Core/Application.h"
#include "Core/Input.h"

#include <glad/glad.h>

namespace Idra {

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:   return GL_FLOAT;
		case ShaderDataType::Float2:  return GL_FLOAT;
		case ShaderDataType::Float3:  return GL_FLOAT;
		case ShaderDataType::Float4:  return GL_FLOAT;
		case ShaderDataType::Mat3:    return GL_FLOAT;
		case ShaderDataType::Mat4:    return GL_FLOAT;
		case ShaderDataType::Int:     return GL_INT;
		case ShaderDataType::Int2:    return GL_INT;
		case ShaderDataType::Int3:    return GL_INT;
		case ShaderDataType::Int4:    return GL_INT;
		case ShaderDataType::Bool:    return GL_BOOL;
		}
		IDRA_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	Application::Application()
	{
		IDRA_WARN("Application Created"); // #DEBUG
		IDRA_ASSERT(!s_Instance, "Application already exists!"); // #DEBUG
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(IDRA_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex data: position (x, y, z), colour (r, g, b, a)
		float vertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.2f, 0.8f, 0.5f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.6f, 0.4f, 0.4f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout bufferlayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(bufferlayout);

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout) 
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				m_VertexBuffer->GetLayout().GetStride(),
				(const void*)element.Offset);
			index++;
		}

		uint32_t indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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

			in vec4 v_Color;
			in vec3 v_Position;

			out vec4 color;

			void main()
			{
				color = v_Color;
				//color = vec4(v_Position * 0.5 + 0.5, 1.0); //static color based on position
				//color = vec4(0.8, 0.2, 0.3, 1.0); //static uniform color

			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc, "BasicShader");
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
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

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