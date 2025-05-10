#pragma once

#include "Core/Core.h"

#include "Renderer/CameraController.h"

#include <glm/glm.hpp>

namespace Idra {
	class IDRA_API EditorCameraController : public CameraController
	{
	public:
		EditorCameraController(bool isCameraLooking = false, bool isCameraMoving = true);
		~EditorCameraController() override;

		void OnUpdate(Camera& camera, Timestep ts) override;
		void OnEvent(Camera& camera, Event& e) override;

		const bool IsCameraLooking() const { return m_IsCameraLooking; }
		void SetCameraLooking(bool looking) { m_IsCameraLooking = looking; }

		const glm::vec2& GetLastMousePos() const { return m_LastMousePos; }
		void SetLastMousePos(const glm::vec2& pos) { m_LastMousePos = pos; }

	private:
		void ProcessMouseInput(Camera& camera, Timestep ts);
		void ProcessKeyInput(Camera& camera, Timestep ts);

	private:
		bool m_IsCameraLooking;
		bool m_IsCameraMoving;

		glm::vec2 m_LastMousePos = { 0.0f, 0.0f };
	};
}