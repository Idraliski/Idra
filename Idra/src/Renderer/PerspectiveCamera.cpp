#include "IdraPCH.h"

#include "Renderer/PerspectiveCamera.h"

namespace Idra {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: Camera(nearClip, farClip), m_FOV(fov), m_AspectRatio(aspectRatio)
	{
		IDRA_PROFILE_FUNCTION();

		m_ProjectionType = CameraProjectionType::Perspective;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetZoomLevel(float zoomLevel)
	{
		IDRA_PROFILE_FUNCTION();

		float BaseFOV = 45.0f;
		m_ZoomLevel = glm::clamp(zoomLevel, 0.30f, 10.0f);
		SetFOV(BaseFOV / m_ZoomLevel);
	}

	void PerspectiveCamera::OnWindowResize(float width, float height)
	{
		IDRA_PROFILE_FUNCTION();

		m_AspectRatio = width / height;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		IDRA_PROFILE_FUNCTION();

		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		IDRA_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}