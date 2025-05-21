#include "IdraPCH.h"

#include "Renderer/PerspectiveCamera.h"

namespace Idra {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: Camera(nearClip, farClip), m_FOV(fov), m_AspectRatio(aspectRatio)
	{
		m_ProjectionType = CameraProjectionType::Perspective;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetZoomLevel(float zoomLevel)
	{
		float BaseFOV = 45.0f;
		m_ZoomLevel = glm::clamp(zoomLevel, 0.30f, 10.0f);
		SetFOV(BaseFOV / m_ZoomLevel);
	}

	void PerspectiveCamera::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}