#include "ExampleLayer.h"

//--TEMP--
#include "Platform/OpenGL/OpenGLShader.h"
//--------

#include <Math/MathFormatters.h>
#include <imgui.h>
#include <memory>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	auto width = Idra::Application::Get().GetWindow().GetWidth();
	auto height = Idra::Application::Get().GetWindow().GetHeight();

	Idra::PerspectiveCameraSpec perspCameraSpec;
	perspCameraSpec.FOV = 45.0f;
	perspCameraSpec.AspectRatio = (float)width / (float)height;
	perspCameraSpec.NearClip = 0.1f;
	perspCameraSpec.FarClip = 1'000.0f;

	Idra::OrthographicCameraSpec orthoCameraSpec;
	orthoCameraSpec.Left = -1.0f;
	orthoCameraSpec.Right = 1.0f;
	orthoCameraSpec.Bottom = -1.0f;
	orthoCameraSpec.Top = 1.0f;
	orthoCameraSpec.NearClip = 0.1f;
	orthoCameraSpec.FarClip = 1'000.0f;

	m_Camera.reset(Idra::Camera::CreateCamera(Idra::CameraProjectionType::Perspective, &perspCameraSpec));
	//m_Camera.reset(Idra::Camera::CreateCamera(Idra::CameraProjectionType::Orthographic, &orthoCameraSpec));

	// TEMP DRAW DATA
	m_VertexArray.reset(Idra::VertexArray::Create());

	// 8 vertices: position (x, y, z), color (r, g, b, a)
	float cubeVertices[8 * 7] = {
		// Front face
		-0.75f, -0.75f,  -5.0f,  0.8f, 0.2f, 0.8f, 1.0f, // 0
		 0.75f, -0.75f,  -5.0f,  0.2f, 0.3f, 0.8f, 1.0f, // 1
		 0.75f,  0.75f,  -5.0f,  0.2f, 0.8f, 0.5f, 1.0f, // 2
		-0.75f,  0.75f,  -5.0f,  0.6f, 0.4f, 0.4f, 1.0f, // 3

		// Back face
		-0.75f, -0.75f,  -3.5f,  0.5f, 0.1f, 0.9f, 1.0f, // 4
		 0.75f, -0.75f,  -3.5f,  0.9f, 0.7f, 0.2f, 1.0f, // 5
		 0.75f,  0.75f,  -3.5f,  0.2f, 0.6f, 0.3f, 1.0f, // 6
		-0.75f,  0.75f,  -3.5f,  0.3f, 0.2f, 0.7f, 1.0f  // 7
	};

	std::shared_ptr<Idra::VertexBuffer> squareVB;
	squareVB.reset(Idra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)));
	Idra::BufferLayout bufferlayout = {
		{ Idra::ShaderDataType::Float3, "a_Position" },
		{ Idra::ShaderDataType::Float4, "a_Color" }
	};
	squareVB->SetLayout(bufferlayout);
	m_VertexArray->AddVertexBuffer(squareVB);

	unsigned int cubeIndices[36] = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Right face
		1, 5, 6, 6, 2, 1,
		// Back face
		5, 4, 7, 7, 6, 5,
		// Left face
		4, 0, 3, 3, 7, 4,
		// Top face
		3, 2, 6, 6, 7, 3,
		// Bottom face
		4, 5, 1, 1, 0, 4
	};
	std::shared_ptr<Idra::IndexBuffer> squareIB;
	squareIB.reset(Idra::IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(squareIB);

	m_TriVA.reset(Idra::VertexArray::Create());
	// 6 vertices: front + back (x, y, z)
	float prismVertices[6 * 3] = {
		// Front face
		-0.5f, -0.5f, -1.0f,
		 0.5f, -0.5f, -1.0f,
		 0.0f,  0.5f, -1.0f,

		 // Back face
		 -0.5f, -0.5f, -3.0f,
		  0.5f, -0.5f, -3.0f,
		  0.0f,  0.5f, -3.0f
	};

	std::shared_ptr<Idra::VertexBuffer> triVB;
	triVB.reset(Idra::VertexBuffer::Create(prismVertices, sizeof(prismVertices)));
	triVB->SetLayout({
		{ Idra::ShaderDataType::Float3, "a_Position" }
		});
	m_TriVA->AddVertexBuffer(triVB);

	unsigned int prismIndices[24] = {
		// Front triangle
		0, 1, 2,

		// Back triangle
		3, 5, 4,

		// Side 1
		0, 2, 5,
		5, 3, 0,

		// Side 2
		1, 4, 5,
		5, 2, 1,

		// Side 3 (bottom)
		0, 3, 1,
		1, 3, 4
	};
	std::shared_ptr<Idra::IndexBuffer> triIB;
	triIB.reset(Idra::IndexBuffer::Create(prismIndices, sizeof(prismIndices) / sizeof(uint32_t)));
	m_TriVA->SetIndexBuffer(triIB);

	// TEMP
	// Load and compile the vertex shader
	std::string vertexSrc = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;
			out vec3 v_Position;

			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

	// Load and compile the fragment shader
	// This is a simple shader that just outputs the texture color
	std::string fragmentSrc = R"(
			#version 460 core

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

	m_Shader.reset(Idra::Shader::Create(vertexSrc, fragmentSrc));

	// TEMP
	// Load and compile the vertex shader
	std::string flatColourVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

	// Load and compile the fragment shader
	// This is a simple shader that just outputs the texture color
	std::string flatColourFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			uniform vec3 v_Color;

			void main()
			{
				color = vec4(v_Color, 1.0);
			}
		)";

	m_FlatColourShader.reset(Idra::Shader::Create(flatColourVertexSrc, flatColourFragmentSrc));
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate(Idra::Timestep ts) 
{
	ProcessKeyInput(ts);

	Idra::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	Idra::RenderCommand::Clear();

	// Ideally, we would have a Renderer class that handles all the rendering
	//Renderer::BeginScene(camera, lights, environment);

	Idra::Renderer::BeginScene(m_Camera);
	Idra::Renderer::Submit(m_Shader ,m_VertexArray);
	m_FlatColourShader->Bind();
	std::dynamic_pointer_cast<Idra::OpenGLShader>(m_FlatColourShader)->SetUniform3f("v_Color", m_Colour);
	Idra::Renderer::Submit(m_FlatColourShader, m_TriVA);
	Idra::Renderer::EndScene();
}

/*
* If built as DLL, ImGui context is not shared across DLL boundaries.
* Static Library builds are fine.
* If we want to DLL, switch the MSVC to use DLL runtime library, in CMake
*/
void ExampleLayer::OnImGuiRender(Idra::Timestep ts)
{
	IDRA_ASSERT(ImGui::GetCurrentContext(), "No ImGui context available!");

	// refresh FPS only after the FPS update interval
	if (m_FPSUpdateCounter > m_FPSUpdateInterval)
	{
		m_FPSUpdateCounter = 0.0f;
		m_CurrentFPS = 1.0f / ts;
	}
	else
	{
		m_FPSUpdateCounter += ts;
	}

	ImGui::Begin("Main Menu");
	ImGui::Text("FPS: %.1f", m_CurrentFPS);
	if (ImGui::TreeNode("RendererAPI Info: "))
	{
		ImGui::Text("  Vendor: %s", Idra::Application::Get().GetWindow().GetVendor().c_str());
		ImGui::Text("  Renderer: %s", Idra::Application::Get().GetWindow().GetRenderer().c_str());
		ImGui::Text("  Version: %s", Idra::Application::Get().GetWindow().GetVersion().c_str());
		ImGui::TreePop();
	}
	if(ImGui::TreeNode("Camera")) 
	{
		ImGui::Text("Camera Position: %.2f, %.2f, %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
		ImGui::Text("Camera Rotation: %.2f, %.2f, %.2f", m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
		ImGui::TreePop();
	}
	ImGui::ColorEdit3("Triangle Colour", glm::value_ptr(m_Colour));
	ImGui::End();

	/** some ideas for future on rendering a viewport in imgui
	// Render scene to an offscreen framebuffer (FBO)
	ImTextureID sceneTexture = (ImTextureID)(intptr_t)myFramebuffer->GetColorAttachmentRendererID();

	ImGui::Begin("Scene");
	ImGui::Image(sceneTexture, ImVec2(width, height));
	ImGui::End();
	*/
}

void ExampleLayer::OnAttach()
{
	IDRA_INFO("Example Layer Attached"); // #DEBUG
}

void ExampleLayer::OnDetach()
{
	IDRA_INFO("Example Layer Detached"); // #DEBUG
}

void ExampleLayer::OnEvent(Idra::Event& e) 
{
	if (e.GetEventType() != Idra::EventType::MouseMoved)
		IDRA_TRACE("Example Layer: {0}", e); // #DEBUG
}

void ExampleLayer::ProcessKeyInput(Idra::Timestep ts)
{
	float moveSpeed = 5.0f * ts;
	float rotateSpeed = 50.0f * ts;

	if (Idra::Input::IsKeyPressed(IDRA_KEY_W))
	{
		m_Camera->SetPosition(m_Camera->GetPosition() + moveSpeed * m_Camera->GetForward());
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_S))
	{
		m_Camera->SetPosition(m_Camera->GetPosition() - moveSpeed * m_Camera->GetForward());
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_A))
	{
		m_Camera->SetPosition(m_Camera->GetPosition() - moveSpeed * m_Camera->GetRight());
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_D))
	{
		m_Camera->SetPosition(m_Camera->GetPosition() + moveSpeed * m_Camera->GetRight());
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_Q))
	{
		m_Camera->SetRotation(m_Camera->GetRotation() + rotateSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_E))
	{
		m_Camera->SetRotation(m_Camera->GetRotation() - rotateSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_SPACE))
	{
		m_Camera->SetPosition(m_Camera->GetPosition() + moveSpeed * m_Camera->GetUp());
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_LEFT_CONTROL))
	{
		m_Camera->SetPosition(m_Camera->GetPosition() - moveSpeed * m_Camera->GetUp());
	}
}