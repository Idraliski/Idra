#include "Sandbox3DLayer.h"

#include <imgui.h>
#include <memory>
#include <glm/gtc/type_ptr.hpp>

Sandbox3DLayer::Sandbox3DLayer()
	: Layer("3D Layer Example")
{
	IDRA_PROFILE_FUNCTION();

	{
		IDRA_PROFILE_SCOPE("Sandbox3DLayer Camera Init");

		// Camera Init
		m_PerspectiveCamera = Idra::Camera::CreateCamera(Idra::CameraProjectionType::Perspective);
		m_OrthoCamera = Idra::Camera::CreateCamera(Idra::CameraProjectionType::Orthographic);

		m_EditorCameraController = Idra::CameraController::CreateCameraController(Idra::CameraControllerType::EditorCamera);

		// Set the camera position and rotation
		m_PerspectiveCamera->SetPosition({ 0.0f, 0.0f, 10.0f });
		m_PerspectiveCamera->SetRotation({ 0.0f, -1.0f, 0.0f });
		m_OrthoCamera->SetPosition({ 0.0f, 0.0f, 10.0f });
		m_OrthoCamera->SetRotation({ 0.0f, -1.0f, 0.0f });

		m_Camera = m_UsePerspectiveCamera ? m_PerspectiveCamera : m_OrthoCamera;
	}

	{
		IDRA_PROFILE_SCOPE("Sandbox3DLayer Model Init");

		// Load the model
		m_Model_Sphere = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/ico-sphere.obj");
		m_Model_Cube = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/cube.obj");
		m_Model_D20 = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/D20.obj");
		m_Model_WoodBench = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Furniture/WoodBench.obj");
		m_Model_MetalTable = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Furniture/MetalTable.obj");
		m_Model_Skybox = Idra::ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Skybox.obj");
	}

	{
		IDRA_PROFILE_SCOPE("Sandbox3DLayer Texture Init");

		// Load the texture
		m_Texture = Idra::Texture2D::Create("Assets/Textures/ico-sphere.png");
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
	}

	{
		IDRA_PROFILE_SCOPE("Sandbox3DLayer Shader Init");

		// Shaders
		Path BasicGLSL = "Assets/Shaders/Basic.glsl";
		Path textureGLSL = "Assets/Shaders/BasicTexture.glsl";
		Path flatColourVertexSrc = "Assets/Shaders/FlatColour.vert";
		Path flatColourFragmentSrc = "Assets/Shaders/FlatColour.frag";
		Path skyboxGLSL = "Assets/Shaders/Skybox.glsl";

		m_ShaderLibrary.Load(BasicGLSL);
		auto flatColourShader = m_ShaderLibrary.Load("FlatColour", flatColourVertexSrc, flatColourFragmentSrc);
		auto textureShader = m_ShaderLibrary.Load("Texture", textureGLSL);
		textureShader->Bind();
		textureShader->SetUniform1i("u_Texture", 0);
		auto skyboxShader = m_ShaderLibrary.Load("Skybox", skyboxGLSL);
		skyboxShader->Bind();
		skyboxShader->SetUniform1i("u_Skybox", 0);
	}

	// Transforms
	m_Transform_Sphere.Position = { 0.0f, 2.0f, 0.0f };
	m_Transform_Sphere2.Position = { -2.0f, 2.0f, 0.0f };
	m_Transform_Cube.Position = { 0.0f, 0.0f, 0.0f };
	m_Transform_D20.Position = { 0.0f, 0.0f, -2.0f };
	m_Transform_WoodBench.Position = { 10.0f, 0.0f, -20.0f };
	m_Transform_MetalTable.Position = { -10.0f, 0.0f, -10.0f };
}

Sandbox3DLayer::~Sandbox3DLayer()
{
	IDRA_PROFILE_FUNCTION();

	IDRA_INFO("Sandbox 3D Layer Destroyed"); // #DEBUG
}

void Sandbox3DLayer::OnAttach()
{
	IDRA_PROFILE_FUNCTION();

	IDRA_INFO("Sandbox 3D Layer Attached"); // #DEBUG
}

void Sandbox3DLayer::OnDetach()
{
	IDRA_PROFILE_FUNCTION();

	IDRA_INFO("Sandbox 3D Layer Detached"); // #DEBUG
}

void Sandbox3DLayer::OnUpdate(Idra::Timestep ts)
{
	IDRA_PROFILE_FUNCTION();

	ProcessKeyInput(ts);
	ProcessMouseInput(ts);

	// Update the camera
	m_Camera = m_UsePerspectiveCamera ? m_PerspectiveCamera : m_OrthoCamera;
	m_EditorCameraController->OnUpdate(m_Camera, ts);

	{
		IDRA_PROFILE_SCOPE("RenderScene");

		auto basicShader = m_ShaderLibrary.Get("Basic");
		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto flatColourShader = m_ShaderLibrary.Get("FlatColour");
		flatColourShader->Bind();
		flatColourShader->SetUniform3f("v_Color", m_Colour);
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
}

void Sandbox3DLayer::OnImGuiRender(Idra::Timestep ts)
{
	IDRA_PROFILE_FUNCTION();

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
	if (ImGui::TreeNode("Camera"))
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
	ImGui::ColorEdit3("D20 Colour", glm::value_ptr(m_Colour));

	ImGui::End();
}

void Sandbox3DLayer::OnEvent(Idra::Event& e)
{
	IDRA_PROFILE_FUNCTION();

	Idra::EventDispatcher dispatcher(e);
	m_EditorCameraController->OnEvent(m_Camera, e);

	dispatcher.Dispatch<Idra::KeyPressedEvent>(IDRA_BIND_EVENT_FN(Sandbox3DLayer::OnKeyPressed));
}

void Sandbox3DLayer::ProcessKeyInput(Idra::Timestep ts)
{
	IDRA_PROFILE_FUNCTION();

	// TEMP Sphere movement
	float moveSpeed = 1.0f;
	if (Idra::Input::IsKeyPressed(IDRA_KEY_UP))
	{
		m_Transform_Sphere.Position.y += moveSpeed * ts;
		m_Transform_Sphere.Dirty = true; 
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_DOWN))
	{
		m_Transform_Sphere.Position.y -= moveSpeed * ts;
		m_Transform_Sphere.Dirty = true;
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_LEFT))
	{
		m_Transform_Sphere.Position.x -= moveSpeed * ts;
		m_Transform_Sphere.Dirty = true;
	}
	if (Idra::Input::IsKeyPressed(IDRA_KEY_RIGHT))
	{
		m_Transform_Sphere.Position.x += moveSpeed * ts;
		m_Transform_Sphere.Dirty = true;
	}
}

void Sandbox3DLayer::ProcessMouseInput(Idra::Timestep ts)
{
	IDRA_PROFILE_FUNCTION();

}

bool Sandbox3DLayer::OnKeyPressed(Idra::KeyPressedEvent& e)
{
	IDRA_PROFILE_FUNCTION();

	if (e.GetKeyCode() == IDRA_KEY_ESCAPE)
		Idra::Application::Get().Close();

	if (e.GetKeyCode() == IDRA_KEY_F1)
	{
		m_UsePerspectiveCamera = !m_UsePerspectiveCamera;
	}
	return false;
}