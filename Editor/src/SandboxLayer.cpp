#include "SandboxLayer.h"

//--TEMP--
#include "Platform/OpenGL/OpenGLShader.h"
//--------

#include <Math/MathFormatters.h> // to use glm::vec3, glm::vec4, etc. in fmt::format i.e., IDRA_INFO()
#include <imgui.h>
#include <memory>
#include <glm/gtc/type_ptr.hpp>


SandboxLayer::SandboxLayer()
	: Layer("Example")
{
	// Camera Init
	Idra::PerspectiveCameraSpec perspCameraSpec;
	perspCameraSpec.FOV = 45.0f;
	perspCameraSpec.AspectRatio = (float)Idra::Application::Get().GetWindow().GetWidth() / (float)Idra::Application::Get().GetWindow().GetHeight();
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
	m_EditorCameraController.reset(Idra::CameraController::CreateCameraController(Idra::CameraControllerType::EditorCamera));

	// Set the camera position and rotation
	m_Camera->SetPosition({ 3.0f, 2.0f, 10.0f });
	m_Camera->SetRotation({ 0.0f, -1.0f, 0.0f });

	// Load the model
	m_Model_Sphere.reset(Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/ico-sphere.obj"));
	m_Model_Cube.reset(Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/cube.obj"));
	m_Model_D20.reset(Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/D20.obj"));

	m_Texture.reset(Idra::Texture2D::Create("Assets/Models/ico-sphere1.png"));

	// Shaders
	Path vertexSrc = "Assets/Shaders/Basic.vert";
	Path fragmentSrc = "Assets/Shaders/Basic.frag";
	m_Shader.reset(Idra::Shader::Create(vertexSrc, fragmentSrc));

	Path textureVertexSrc = "Assets/Shaders/BasicTexture.vert";
	Path textureFragmentSrc = "Assets/Shaders/BasicTexture.frag";
	m_TextureShader.reset(Idra::Shader::Create(textureVertexSrc, textureFragmentSrc));

	std::dynamic_pointer_cast<Idra::OpenGLShader>(m_TextureShader)->Bind();
	std::dynamic_pointer_cast<Idra::OpenGLShader>(m_TextureShader)->SetUniform1i("u_Texture", 0);

	Path flatColourVertexSrc = "Assets/Shaders/FlatColour.vert";
	Path flatColourFragmentSrc = "Assets/Shaders/FlatColour.frag";
	m_FlatColourShader.reset(Idra::Shader::Create(flatColourVertexSrc, flatColourFragmentSrc));
}

SandboxLayer::~SandboxLayer()
{
	IDRA_INFO("Example Layer Destroyed"); // #DEBUG
}

void SandboxLayer::OnUpdate(Idra::Timestep ts)
{
	ProcessKeyInput(ts);
	ProcessMouseInput(ts);

	// Update the camera
	m_EditorCameraController->OnUpdate(*m_Camera, ts);

	Idra::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	Idra::RenderCommand::Clear();

	// Ideally, we would have a Renderer class that handles all the rendering
	//Renderer::BeginScene(camera, lights, environment);

	Idra::Renderer::BeginScene(m_Camera);
	//Idra::Renderer::Submit(m_Shader, m_Model_Cube);

	m_Texture->Bind();
	Idra::Renderer::Submit(m_TextureShader, m_Model_Sphere);

	/*
	m_FlatColourShader->Bind();
	std::dynamic_pointer_cast<Idra::OpenGLShader>(m_FlatColourShader)->SetUniform3f("v_Color", m_Colour);
	Idra::Renderer::Submit(m_FlatColourShader, m_Model_D20);
	*/

	Idra::Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender(Idra::Timestep ts)
{
	IDRA_ASSERT(ImGui::GetCurrentContext(), "No ImGui context available!");

	// @TODO: delete demo window at some point
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

	// refresh FPS only after the FPS update interval
	if (m_FPSUpdateCounter > m_FPSUpdateInterval)
	{
		m_FPSUpdateCounter = 0.0f;
		m_CurrentFPS = 1.0f / ts;
	}
	else
		m_FPSUpdateCounter += ts;

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

void SandboxLayer::OnAttach()
{
	IDRA_INFO("Example Layer Attached"); // #DEBUG
}

void SandboxLayer::OnDetach()
{
	IDRA_INFO("Example Layer Detached"); // #DEBUG
}

void SandboxLayer::OnEvent(Idra::Event& e)
{
	Idra::EventDispatcher dispatcher(e);
	m_EditorCameraController->OnEvent(*m_Camera, e);

	dispatcher.Dispatch<Idra::KeyPressedEvent>(IDRA_BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
}

void SandboxLayer::ProcessKeyInput(Idra::Timestep ts)
{
	
}

void SandboxLayer::ProcessMouseInput(Idra::Timestep ts)
{
	
}

bool SandboxLayer::OnKeyPressed(Idra::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == IDRA_KEY_ESCAPE)
		Idra::Application::Get().SetRunning(false);
	return false;
}