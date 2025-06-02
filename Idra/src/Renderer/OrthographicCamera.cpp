#include "IdraPCH.h"

#include "Renderer/OrthographicCamera.h"

namespace Idra {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip)
		: Camera(nearClip, farClip), m_AspectRatio((right - left) / (top - bottom))
	{
		IDRA_PROFILE_FUNCTION();

		m_ProjectionType = CameraProjectionType::Orthographic;
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void OrthographicCamera::SetZoomLevel(float zoomLevel)
	{
		IDRA_PROFILE_FUNCTION();

		m_ZoomLevel = glm::clamp(zoomLevel, 0.1f, 50.0f);

		UpdateProjectionMatrix();
	}

	void OrthographicCamera::OnWindowResize(float width, float height)
	{
		IDRA_PROFILE_FUNCTION();

		m_AspectRatio = width / height;
		UpdateProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(float aspectRatio, float nearClip, float farClip)
	{
		IDRA_PROFILE_FUNCTION();

		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		UpdateProjectionMatrix();
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		IDRA_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * 1/m_ZoomLevel, m_AspectRatio * 1/m_ZoomLevel, -1/m_ZoomLevel, 1/m_ZoomLevel, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}