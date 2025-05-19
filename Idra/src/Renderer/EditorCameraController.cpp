#include "IdraPCH.h"

#include "Renderer/EditorCameraController.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"

namespace Idra {
	EditorCameraController::EditorCameraController(bool isCameraLooking, bool isCameraMoving)
		: m_IsCameraLooking(isCameraLooking), m_IsCameraMoving(isCameraMoving)
	{

	}

	EditorCameraController::~EditorCameraController()
	{

	}

	void EditorCameraController::OnUpdate(Ref<Camera> camera, Timestep ts)
	{
		ProcessKeyInput(camera, ts);
		ProcessMouseMove(camera, ts);
	}

	void EditorCameraController::OnEvent(Ref<Camera> camera, Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<MouseButtonPressedEvent>(IDRA_BIND_EVENT_FN(EditorCameraController::OnMouseButtonPressed));
		Dispatcher.Dispatch<MouseButtonReleasedEvent>(IDRA_BIND_EVENT_FN(EditorCameraController::OnMouseButtonReleased));
	}

	bool EditorCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == IDRA_MOUSE_BUTTON_2)
		{
			m_IsCameraLooking = true;

			auto mousePos = Idra::Input::GetMousePosition();
			m_LastMousePos = { mousePos.first, mousePos.second };

			Idra::Application::Get().GetWindow().SetCursorMode(Idra::CursorMode::Disabled);
			return true;
		}

		return false;
	}

	bool EditorCameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == IDRA_MOUSE_BUTTON_2)
		{
			m_IsCameraLooking = false;

			Idra::Application::Get().GetWindow().SetCursorMode(Idra::CursorMode::Normal);
			return true;
		}

		return false;
	}

	void EditorCameraController::ProcessMouseMove(Ref<Camera> camera, Timestep ts)
	{
		if (!m_IsCameraLooking)
			return;

		float rotateSpeed = camera->GetRotationSpeed() * ts;

		auto currMousePos = Idra::Input::GetMousePosition();
		glm::vec2 mousePos = { currMousePos.first, currMousePos.second };
		glm::vec2 delta = m_LastMousePos - mousePos;
		m_LastMousePos = mousePos;

		camera->SetRotation(camera->GetRotation() + rotateSpeed * glm::vec3(delta.y, delta.x, 0.0f));
	}

	void EditorCameraController::ProcessKeyInput(Ref<Camera> camera, Timestep ts)
	{
		if (!m_IsCameraMoving)
			return;

		float moveSpeed = camera->GetMovementSpeed() * ts;

		if (Idra::Input::IsKeyPressed(IDRA_KEY_W))
			camera->SetPosition(camera->GetPosition() + camera->GetForward() * moveSpeed);
		if (Idra::Input::IsKeyPressed(IDRA_KEY_S))
			camera->SetPosition(camera->GetPosition() - camera->GetForward() * moveSpeed);
		if (Idra::Input::IsKeyPressed(IDRA_KEY_A))
			camera->SetPosition(camera->GetPosition() - camera->GetRight() * moveSpeed);
		if (Idra::Input::IsKeyPressed(IDRA_KEY_D))
			camera->SetPosition(camera->GetPosition() + camera->GetRight() * moveSpeed);
		if (Idra::Input::IsKeyPressed(IDRA_KEY_SPACE))
			camera->SetPosition(camera->GetPosition() + camera->GetUp() * moveSpeed);
		if (Idra::Input::IsKeyPressed(IDRA_KEY_LEFT_CONTROL))
			camera->SetPosition(camera->GetPosition() - camera->GetUp() * moveSpeed);
	}
}