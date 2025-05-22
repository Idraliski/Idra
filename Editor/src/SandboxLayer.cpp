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
	Idra::OrthographicCameraSpec orthoCameraSpec;

	m_PerspectiveCamera = Idra::Camera::CreateCamera(Idra::CameraProjectionType::Perspective, &perspCameraSpec);
	m_OrthoCamera = Idra::Camera::CreateCamera(Idra::CameraProjectionType::Orthographic, &orthoCameraSpec);
	m_EditorCameraController = Idra::CameraController::CreateCameraController(Idra::CameraControllerType::EditorCamera);

	// Set the camera position and rotation
	m_PerspectiveCamera->SetPosition({ 0.0f, 0.0f, 10.0f });
	m_PerspectiveCamera->SetRotation({ 0.0f, -1.0f, 0.0f });
	m_OrthoCamera->SetPosition({ 0.0f, 0.0f, 10.0f });
	m_OrthoCamera->SetRotation({ 0.0f, -1.0f, 0.0f });

	m_Camera = m_UsePerspectiveCamera ? m_PerspectiveCamera : m_OrthoCamera;

	// Load the model
	m_Model_Sphere = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/ico-sphere.obj");
	m_Model_Cube = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/cube.obj");
	m_Model_D20 = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/D20.obj");
	m_Model_WoodBench = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Furniture/WoodBench.obj");
	m_Model_MetalTable = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Furniture/MetalTable.obj");
	m_Model_Skybox = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Skybox.obj");

	// Load the texture
	m_Texture = Idra::Texture2D::Create("Assets/Textures/default.png");
	m_AlphaTexture = Idra::Texture2D::Create("Assets/Textures/Alpha.png");
	m_Texture_MetalTable = Idra::Texture2D::Create("Assets/Textures/MetalSteelWorn.jpg");
	m_Texture_WoodBench = Idra::Texture2D::Create("Assets/Textures/WoodBench.png");
	m_Texture_Skybox = Idra::TextureCube::Create(
		{ 
			"Assets/Textures/Skybox/default/right.jpg",
			"Assets/Textures/Skybox/default/left.jpg",
			"Assets/Textures/Skybox/default/top.jpg",
			"Assets/Textures/Skybox/default/bottom.jpg",
			"Assets/Textures/Skybox/default/front.jpg",
			"Assets/Textures/Skybox/default/back.jpg"
		}, false
	);

	// Shaders
	Path BasicGLSL = "Assets/Shaders/Basic.glsl";
	Path textureGLSL = "Assets/Shaders/BasicTexture.glsl";
	Path flatColourVertexSrc = "Assets/Shaders/FlatColour.vert";
	Path flatColourFragmentSrc = "Assets/Shaders/FlatColour.frag";
	Path skyboxGLSL = "Assets/Shaders/Skybox.glsl";

	m_ShaderLibrary.Load(BasicGLSL);
	auto flatColourShader = m_ShaderLibrary.Load("FlatColour", flatColourVertexSrc, flatColourFragmentSrc);
	std::dynamic_pointer_cast<Idra::OpenGLShader>(flatColourShader)->Bind();
	std::dynamic_pointer_cast<Idra::OpenGLShader>(flatColourShader)->SetUniform3f("v_Color", m_Colour);
	auto textureShader = m_ShaderLibrary.Load("Texture", textureGLSL);
	std::dynamic_pointer_cast<Idra::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Idra::OpenGLShader>(textureShader)->SetUniform1i("u_Texture", 0);
	auto skyboxShader = m_ShaderLibrary.Load("Skybox", skyboxGLSL);
	std::dynamic_pointer_cast<Idra::OpenGLShader>(skyboxShader)->Bind();
	std::dynamic_pointer_cast<Idra::OpenGLShader>(skyboxShader)->SetUniform1i("u_Skybox", 0);

	// Transforms
	m_Transform_Sphere.Position = { 0.0f, 2.0f, 0.0f };
	m_Transform_Sphere2.Position = { -2.0f, 2.0f, 0.0f };
	m_Transform_Cube.Position = { 0.0f, 0.0f, 0.0f };
	m_Transform_D20.Position = { 0.0f, 0.0f, -2.0f };
	m_Transform_WoodBench.Position = { 10.0f, 0.0f, -20.0f };
	m_Transform_MetalTable.Position = { -10.0f, 0.0f, -10.0f };
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
	m_Camera = m_UsePerspectiveCamera ? m_PerspectiveCamera : m_OrthoCamera;
	m_EditorCameraController->OnUpdate(m_Camera, ts);

	Idra::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	Idra::RenderCommand::Clear();

	// Ideally, we would have a Renderer class that handles all the rendering
	//Renderer::BeginScene(camera, lights, environment);

	auto basicShader = m_ShaderLibrary.Get("Basic");
	auto textureShader = m_ShaderLibrary.Get("Texture");
	auto flatColourShader = m_ShaderLibrary.Get("FlatColour");
	auto skyboxShader = m_ShaderLibrary.Get("Skybox");

	m_Texture_Skybox->Bind();
	Idra::Renderer::BeginScene(m_Camera, skyboxShader, m_Model_Skybox);

	m_Texture->Bind();
	Idra::Renderer::Submit(textureShader, m_Model_Sphere, m_Transform_Sphere);
	m_Texture_MetalTable->Bind();
	Idra::Renderer::Submit(textureShader, m_Model_MetalTable, m_Transform_MetalTable);
	m_Texture_WoodBench->Bind();
	Idra::Renderer::Submit(textureShader, m_Model_WoodBench, m_Transform_WoodBench);

	Idra::Renderer::Submit(basicShader, m_Model_Sphere, m_Transform_Sphere2);
	Idra::Renderer::Submit(flatColourShader, m_Model_D20, m_Transform_D20);
	
	// anything with an alpha texture needs to be rendered last? Even after Skybox
	// might be worth creating a SubmitTransparent function
	m_AlphaTexture->Bind();
	Idra::Renderer::Submit(textureShader, m_Model_Cube, m_Transform_Cube);

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
		ImGui::Text("Camera Type: %s", m_UsePerspectiveCamera ? "Perspective" : "Orthographic");
		ImGui::Text("Camera Position: %.2f, %.2f, %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
		ImGui::Text("Camera Rotation: %.2f, %.2f, %.2f", m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
		ImGui::Text("Camera Zoom Level: %.2f", m_Camera->GetZoomLevel());
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Model"))
	{
		ImGui::Text("Model Loader Type: %s", Idra::ModelLoader::ModelLoaderTypeToString(m_ModelLoaderType).c_str());
		ImGui::Text("Model %s Position: %.2f, %.2f, %.2f", m_Model_Sphere->GetName().c_str(), m_Transform_Sphere.Position.x, m_Transform_Sphere.Position.y, m_Transform_Sphere.Position.z);
		ImGui::Text("Model %s Position: %.2f, %.2f, %.2f", m_Model_Cube->GetName().c_str(), m_Transform_Cube.Position.x, m_Transform_Cube.Position.y, m_Transform_Cube.Position.z);
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
	m_EditorCameraController->OnEvent(m_PerspectiveCamera, e);
	m_EditorCameraController->OnEvent(m_OrthoCamera, e);

	dispatcher.Dispatch<Idra::KeyPressedEvent>(IDRA_BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
}

void SandboxLayer::ProcessKeyInput(Idra::Timestep ts)
{
	// TEMP Sphere movement
	float moveSpeed = 1.0f;
	if (Idra::Input::IsKeyPressed(IDRA_KEY_UP))
		m_Transform_Sphere.Position.y += moveSpeed * ts;
	if (Idra::Input::IsKeyPressed(IDRA_KEY_DOWN))
		m_Transform_Sphere.Position.y -= moveSpeed * ts;
	if (Idra::Input::IsKeyPressed(IDRA_KEY_LEFT))
		m_Transform_Sphere.Position.x -= moveSpeed * ts;
	if (Idra::Input::IsKeyPressed(IDRA_KEY_RIGHT))
		m_Transform_Sphere.Position.x += moveSpeed * ts;
}

void SandboxLayer::ProcessMouseInput(Idra::Timestep ts)
{
	
}

bool SandboxLayer::OnKeyPressed(Idra::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == IDRA_KEY_ESCAPE)
		Idra::Application::Get().SetRunning(false);

	if (e.GetKeyCode() == IDRA_KEY_F1)
	{
		m_UsePerspectiveCamera = !m_UsePerspectiveCamera;
	}
	return false;
}