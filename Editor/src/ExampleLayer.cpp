#include "ExampleLayer.h"

#include <glm/glm.hpp>
#include <Math/MathFormatters.h>
#include <imgui.h>
#include <memory>

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	// TEMP DRAW DATA
	m_VertexArray.reset(Idra::VertexArray::Create());

	// Vertex data: position (x, y, z), colour (r, g, b, a)
	float vertices[4 * 7] = {
		-0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.75f, -0.75f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.75f,  0.75f, 0.0f, 0.2f, 0.8f, 0.5f, 1.0f,
		-0.75f,  0.75f, 0.0f, 0.6f, 0.4f, 0.4f, 1.0f,
	};

	std::shared_ptr<Idra::VertexBuffer> squareVB;
	squareVB.reset(Idra::VertexBuffer::Create(vertices, sizeof(vertices)));
	Idra::BufferLayout bufferlayout = {
		{ Idra::ShaderDataType::Float3, "a_Position" },
		{ Idra::ShaderDataType::Float4, "a_Color" }
	};
	squareVB->SetLayout(bufferlayout);
	m_VertexArray->AddVertexBuffer(squareVB);

	uint32_t indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
	std::shared_ptr<Idra::IndexBuffer> squareIB;
	squareIB.reset(Idra::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(squareIB);

	m_TriVA.reset(Idra::VertexArray::Create());
	float triVertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};
	std::shared_ptr<Idra::VertexBuffer> triVB;
	triVB.reset(Idra::VertexBuffer::Create(triVertices, sizeof(triVertices)));
	triVB->SetLayout({
		{ Idra::ShaderDataType::Float3, "a_Position" }
		});
	m_TriVA->AddVertexBuffer(triVB);

	uint32_t triIndices[3] = {
		0, 1, 2
	};
	std::shared_ptr<Idra::IndexBuffer> triIB;
	triIB.reset(Idra::IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));
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

	m_Shader = std::make_unique<Idra::Shader>(vertexSrc, fragmentSrc, "BasicShader");

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

	m_BlueShader = std::make_unique<Idra::Shader>(blueVertexSrc, blueFragmentSrc, "BasicBlueShader");
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate() 
{
	if (Idra::Input::IsKeyPressed(IDRA_KEY_TAB))
		IDRA_TRACE("Tab key is pressed! (Poll!)"); // #DEBUG

	Idra::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	Idra::RenderCommand::Clear();

	// Ideally, we would have a Renderer class that handles all the rendering
	//Renderer::BeginScene(camera, lights, environment);

	Idra::Renderer::BeginScene();
	m_Shader->Bind();
	Idra::Renderer::Submit(m_VertexArray);
	m_BlueShader->Bind();
	Idra::Renderer::Submit(m_TriVA);
	Idra::Renderer::EndScene();
}

/*
* If built as DLL, ImGui context is not shared across DLL boundaries.
* Static Library builds are fine.
* If we want to DLL, switch the MSVC to use DLL runtime library, in CMake
*/
void ExampleLayer::OnImGuiRender()
{
	IDRA_ASSERT(ImGui::GetCurrentContext(), "No ImGui context available!");

	ImGui::Begin("Example Layer");
	ImGui::Text("Hello from Example Layer!");
	ImGui::End();
}

void ExampleLayer::OnAttach()
{
	IDRA_INFO("Example Layer Attached"); // #DEBUG

	glm::vec3 test = { 1.0f, 2.0f, 3.0f };
	IDRA_INFO("Test: {0}", test); // #DEBUG

	glm::vec2 testVec2 = { 1.0f, 2.0f };
	IDRA_INFO("TestVec2: {0}", testVec2); // #DEBUG

	glm::vec4 testVec4 = { 1.0f, 2.0f, 3.0f, 4.0f };
	IDRA_INFO("TestVec4: {0}", testVec4); // #DEBUG

	glm::mat3 testMat3 = { 1.0f, 2.0f, 3.0f,
						  4.0f, 5.0f, 6.0f,
						  7.0f, 8.0f, 9.0f };
	IDRA_INFO("TestMat3: {0}", testMat3); // #DEBUG

	glm::mat4 testMat4 = { 1.0f, 2.0f, 3.0f, 4.0f,
						  5.0f, 6.0f, 7.0f, 8.0f,
						  9.0f, 10.0f, 11.0f, 12.0f,
						  13.0f, 14.0f, 15.0f, 16.0f };
	IDRA_INFO("TestMat: {0}", testMat4); // #DEBUG

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