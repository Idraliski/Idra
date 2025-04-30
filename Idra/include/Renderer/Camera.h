#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Idra {

	enum class IDRA_API CameraProjectionType
	{
		None = 0,
		Perspective = 1,
		Orthographic = 2
	};

	struct IDRA_API PerspectiveCameraSpec {
		float FOV = 45.0f;
		float AspectRatio = 16.0f / 9.0f;
		float NearClip = 0.1f;
		float FarClip = 1'000.0f;
	};

	struct IDRA_API OrthographicCameraSpec {
		float Left = -1.0f;
		float Right = 1.0f;
		float Bottom = -1.0f;
		float Top = 1.0f;
		float NearClip = 0.1f;
		float FarClip = 1'000.0f;
	};

	class IDRA_API Camera
	{
	public:
		Camera(float nearClip, float farClip);
		virtual ~Camera() = default;

		/**
		* @brief Create a camera of the specified type
		* @param type The type of camera to create
		* @param spec The camera specification (optional). Based on the type, it can be a PerspectiveCameraSpec or OrthographicCameraSpec.
		* @return A pointer to the created camera, either PerspectiveCamera or OrthographicCamera.
		*/
		static Camera* CreateCamera(CameraProjectionType type, const void* spec = nullptr);

		const float GetNearClip() const { return m_NearClip; }
		void SetNearClip(float nearClip) { m_NearClip = nearClip; UpdateProjectionMatrix(); }

		const float GetFarClip() const { return m_FarClip; }
		void SetFarClip(float farClip) { m_FarClip = farClip; UpdateProjectionMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; SetOrientation(glm::quat(rotation)); }
	protected:
		// Quaternion rotation is used to represent the camera's orientation
		// only used internally to update the view matrix
		// public access will use Euler angles instead with SetRotation, GetRotation
		void SetOrientation(const glm::quat& orientation) { m_Orientation = glm::normalize(orientation); UpdateViewMatrix(); }

		void UpdateViewMatrix();

		virtual void UpdateProjectionMatrix() = 0;
	protected:
		CameraProjectionType m_ProjectionType = CameraProjectionType::None;

		float m_NearClip;
		float m_FarClip;

		glm::mat4 m_ProjectionMatrix = { 1.0f };
		glm::mat4 m_ViewMatrix = { 1.0f };
		glm::mat4 m_ViewProjectionMatrix = { 1.0f };

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, -1.0f };
		glm::quat m_Orientation = { 1.0f, 0.0f, 0.0f, 0.0f };
	};
}