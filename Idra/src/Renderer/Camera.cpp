#include "IdraPCH.h"

#include "Renderer/Camera.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"

namespace Idra {
	Camera::Camera(float nearClip, float farClip, float zoomLevel)
		: m_NearClip(nearClip), m_FarClip(farClip), m_ZoomLevel(zoomLevel)
	{
		IDRA_PROFILE_FUNCTION();

	}

	Ref<Camera> Camera::CreateCamera(CameraProjectionType type, const void* spec)
	{
		switch (type)
		{
			case CameraProjectionType::None:
				IDRA_CORE_ASSERT(false, "CameraProjectionType::None is not supported!");
				return nullptr;

			case CameraProjectionType::Perspective:
			{
				auto pSpec = static_cast<const PerspectiveCameraSpec*>(spec);

				// If spec is null, use default values
				PerspectiveCameraSpec defaultSpec;
				if (!pSpec)
				{
					IDRA_CORE_INFO("CameraProjectionType::Perspective spec is null, using default values!");
					pSpec = &defaultSpec;
				}

				return CreateRef<PerspectiveCamera>(pSpec->FOV, pSpec->AspectRatio, pSpec->NearClip, pSpec->FarClip);
			}

			case CameraProjectionType::Orthographic:
			{
				auto oSpec = static_cast<const OrthographicCameraSpec*>(spec);

				// If spec is null, use default values
				OrthographicCameraSpec defaultSpec;
				if (!oSpec)
				{
					IDRA_CORE_INFO("CameraProjectionType::Orthographic spec is null, using default values!");
					oSpec = &defaultSpec;
				}

				return CreateRef<OrthographicCamera>(oSpec->Left, oSpec->Right, oSpec->Bottom, oSpec->Top, oSpec->NearClip, oSpec->FarClip);
			}
		}

		IDRA_CORE_ASSERT(false, "Unknown camera projection type!");
		return nullptr;
	}

	void Camera::UpdateViewMatrix()
	{
		IDRA_PROFILE_FUNCTION();

		// Update the view matrix based on the camera's position and orientation
		// The view matrix is the inverse of the camera's transformation matrix
		// The other common option is to use glm::lookAt, but this is more efficient and flexible
		// ensures no double up on the orientation with ECS, like forward and up vector
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);

		// Since OpenGL is column-major, the order is proj * view * model
		// if its row-major, the order is model * view * proj
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		IDRA_PROFILE_FUNCTION();

		// @TODO Move this to a systems function for all rotations on entities
		auto wrap = [](float angle) {
			return glm::mod(angle + 180.0f, 360.0f) - 180.0f;
		};
		m_Rotation = rotation; 
		// clamp rotation to 360 degrees
		m_Rotation.x = glm::clamp(wrap(rotation.x), -89.0f, 89.0f);
		m_Rotation.y = wrap(rotation.y);
		m_Rotation.z = wrap(rotation.z);

		SetOrientation(glm::quat(glm::radians(rotation))); 
	}
}