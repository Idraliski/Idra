#include "IdraPCH.h"

#include "Renderer/OrthographicCamera.h"

namespace Idra {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip)
		: Camera(nearClip, farClip), m_AspectRatio((right - left) / (top - bottom))
	{
		m_ProjectionType = CameraProjectionType::Orthographic;
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void OrthographicCamera::SetZoomLevel(float zoomLevel)
	{
		m_ZoomLevel = glm::clamp(zoomLevel, 0.1f, 50.0f);

		UpdateProjectionMatrix();
	}

	void OrthographicCamera::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		UpdateProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(float aspectRatio, float nearClip, float farClip)
	{
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		UpdateProjectionMatrix();
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * 1/m_ZoomLevel, m_AspectRatio * 1/m_ZoomLevel, -1/m_ZoomLevel, 1/m_ZoomLevel, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}