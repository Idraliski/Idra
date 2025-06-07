#include "EditorLayer.h"

#include <imgui.h>
#include <memory>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

namespace Idra {
	using Path = std::filesystem::path;

	EditorLayer::EditorLayer()
		: Layer("Idra Editor Layer")
	{
		IDRA_PROFILE_FUNCTION();

		{
			IDRA_PROFILE_SCOPE("Sandbox3DLayer Camera Init");

			// Camera Init
			m_PerspectiveCamera = Camera::CreateCamera(CameraProjectionType::Perspective);
			m_OrthoCamera = Camera::CreateCamera(CameraProjectionType::Orthographic);

			m_EditorCameraController = CameraController::CreateCameraController(CameraControllerType::EditorCamera);

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
			m_Model_Sphere = ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/ico-sphere.obj");
			m_Model_Cube = ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/cube.obj");
			m_Model_D20 = ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/D20.obj");
			m_Model_WoodBench = ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Furniture/WoodBench.obj");
			m_Model_MetalTable = ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Furniture/MetalTable.obj");
			m_Model_Skybox = ModelLoader::LoadModel(m_ModelLoaderType, "Assets/Models/Skybox.obj");
		}

		{
			IDRA_PROFILE_SCOPE("Sandbox3DLayer Texture Init");

			// Load the texture
			m_Texture = Texture2D::Create("Assets/Textures/ico-sphere.png");
			m_AlphaTexture = Texture2D::Create("Assets/Textures/Alpha.png");
			m_Texture_MetalTable = Texture2D::Create("Assets/Textures/MetalSteelWorn.jpg");
			m_Texture_WoodBench = Texture2D::Create("Assets/Textures/WoodBench.png");
			m_Texture_Skybox = TextureCube::Create(
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

		{
			IDRA_PROFILE_SCOPE("Framebuffer Init");

			// Frame Buffer
			FrameBufferSpecification fbSpec;
			fbSpec.Width = Application::Get().GetWindow().GetWidth();
			fbSpec.Height = Application::Get().GetWindow().GetHeight();

			m_FrameBuffer = FrameBuffer::Create(fbSpec);
		}
	}

	EditorLayer::~EditorLayer()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_INFO("Editor Layer Destroyed"); // #DEBUG
	}

	void EditorLayer::OnAttach()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_INFO("Editor Layer Attached"); // #DEBUG
	}

	void EditorLayer::OnDetach()
	{
		IDRA_PROFILE_FUNCTION();

		IDRA_INFO("Editor Layer Detached"); // #DEBUG
	}

	void EditorLayer::OnUpdate(Timestep ts)
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

			// Bind the frame buffer
			m_FrameBuffer->Bind();

			m_Texture_Skybox->Bind();
			Renderer::BeginScene(m_Camera, skyboxShader, m_Model_Skybox);

			m_Texture->Bind();
			Renderer::Submit(textureShader, m_Model_Sphere, m_Transform_Sphere);
			m_Texture_MetalTable->Bind();
			Renderer::Submit(textureShader, m_Model_MetalTable, m_Transform_MetalTable);
			m_Texture_WoodBench->Bind();
			Renderer::Submit(textureShader, m_Model_WoodBench, m_Transform_WoodBench);

			Renderer::Submit(basicShader, m_Model_Sphere, m_Transform_Sphere2);
			Renderer::Submit(flatColourShader, m_Model_D20, m_Transform_D20);

			// anything with an alpha texture needs to be rendered last? Even after Skybox
			// might be worth creating a SubmitTransparent function
			m_AlphaTexture->Bind();
			Renderer::Submit(textureShader, m_Model_Cube, m_Transform_Cube);

			Renderer::EndScene();
			// Unbind the frame buffer
			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender(Timestep ts)
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

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close", NULL, false, dockspaceOpen != NULL))
					Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Main Menu");
			ImGui::Text("FPS: %.1f", m_CurrentFPS);
			ImGui::Text("Viewport Panel Size: %.0f, %.0f", m_ViewportSize.x, m_ViewportSize.y);
			if (ImGui::TreeNode("RendererAPI Info: "))
			{
				ImGui::Text("  Vendor: %s", Application::Get().GetWindow().GetVendor().c_str());
				ImGui::Text("  Renderer: %s", Application::Get().GetWindow().GetRenderer().c_str());
				ImGui::Text("  Version: %s", Application::Get().GetWindow().GetVersion().c_str());
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Settings"))
			{
				ImGui::Text("Window Width: %d", Application::Get().GetWindow().GetWidth());
				ImGui::Text("Window Height: %d", Application::Get().GetWindow().GetHeight());
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Camera"))
			{
				ImGui::Text("Camera Type: %s", m_UsePerspectiveCamera ? "Perspective" : "Orthographic");
				ImGui::Text("Camera Position: %.2f, %.2f, %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
				ImGui::Text("Camera Rotation: %.2f, %.2f, %.2f", m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
				ImGui::Text("Camera Zoom Level: %.2f", m_Camera->GetZoomLevel());
				ImGui::Checkbox("Use Perspective Camera", &m_UsePerspectiveCamera);
				if (ImGui::Button("Switch Camera Type"))
				{
					m_UsePerspectiveCamera = !m_UsePerspectiveCamera;
					m_Camera = m_UsePerspectiveCamera ? m_PerspectiveCamera : m_OrthoCamera;
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Model"))
			{
				ImGui::Text("Model Loader Type: %s", ModelLoader::ModelLoaderTypeToString(m_ModelLoaderType).c_str());
				ImGui::Text("Model %s Position: %.2f, %.2f, %.2f", m_Model_Sphere->GetName().c_str(), m_Transform_Sphere.Position.x, m_Transform_Sphere.Position.y, m_Transform_Sphere.Position.z);
				ImGui::Text("Model %s Position: %.2f, %.2f, %.2f", m_Model_Cube->GetName().c_str(), m_Transform_Cube.Position.x, m_Transform_Cube.Position.y, m_Transform_Cube.Position.z);
				ImGui::TreePop();
			}
			ImGui::ColorEdit3("D20 Colour", glm::value_ptr(m_Colour));

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Viewport");
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
			{
				m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				m_EditorCameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y, m_PerspectiveCamera);
				m_EditorCameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y, m_OrthoCamera);
			}
			ImGui::Image((ImTextureID)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		IDRA_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		m_EditorCameraController->OnEvent(m_Camera, e);

		dispatcher.Dispatch<KeyPressedEvent>(IDRA_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::ProcessKeyInput(Timestep ts)
	{
		IDRA_PROFILE_FUNCTION();

		// TEMP Sphere movement
		float moveSpeed = 1.0f;
		if (Input::IsKeyPressed(IDRA_KEY_UP))
		{
			m_Transform_Sphere.Position.y += moveSpeed * ts;
			m_Transform_Sphere.Dirty = true;
		}
		if (Input::IsKeyPressed(IDRA_KEY_DOWN))
		{
			m_Transform_Sphere.Position.y -= moveSpeed * ts;
			m_Transform_Sphere.Dirty = true;
		}
		if (Input::IsKeyPressed(IDRA_KEY_LEFT))
		{
			m_Transform_Sphere.Position.x -= moveSpeed * ts;
			m_Transform_Sphere.Dirty = true;
		}
		if (Input::IsKeyPressed(IDRA_KEY_RIGHT))
		{
			m_Transform_Sphere.Position.x += moveSpeed * ts;
			m_Transform_Sphere.Dirty = true;
		}
	}

	void EditorLayer::ProcessMouseInput(Timestep ts)
	{
		IDRA_PROFILE_FUNCTION();

	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		IDRA_PROFILE_FUNCTION();

		if (e.GetKeyCode() == IDRA_KEY_F1)
		{
			m_UsePerspectiveCamera = !m_UsePerspectiveCamera;
		}
		return false;
	}
}