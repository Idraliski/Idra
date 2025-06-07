#pragma once

#include "Core/Core.h"

#include "Renderer/CameraController.h"

#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace Idra {
	class IDRA_API EditorCameraController : public CameraController
	{
	public:
		EditorCameraController(bool isCameraMoving = true);
		~EditorCameraController() override;

		void OnUpdate(Ref<Camera> camera, Timestep ts) override;
		void OnEvent(Ref<Camera> camera, Event& e) override;

		const bool IsCameraLooking() const { return m_IsCameraLooking; }
		void SetCameraLooking(bool looking) { m_IsCameraLooking = looking; }

		const glm::vec2& GetLastMousePos() const { return m_LastMousePos; }
		void SetLastMousePos(const glm::vec2& pos) { m_LastMousePos = pos; }

		void OnResize(uint32_t width, uint32_t height, const Ref<Camera>& camera) override;

	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e, const Ref<Camera>& camera);
		bool OnWindowResize(WindowResizeEvent& e, const Ref<Camera>& camera);

		void ProcessMouseMove(Ref<Camera> camera, Timestep ts);
		void ProcessKeyInput(Ref<Camera> camera, Timestep ts);

	private:
		bool m_IsCameraLooking;
		bool m_IsCameraMoving;

		glm::vec2 m_LastMousePos = { 0.0f, 0.0f };

		// Movement data
		// @TODO: Move this to a component system once ECS is implemented
		float m_MovementSpeed = 5.0f;
		float m_RotationSpeed = 50.0f;
		float m_ZoomSpeed = 0.25f;
	};
}