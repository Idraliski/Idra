#include "IdraPCH.h"

#include "Renderer/OrthographicCamera.h"

namespace Idra {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip)
		: Camera(nearClip, farClip), m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
	{
		m_ProjectionType = CameraProjectionType::Orthographic;
		UpdateProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float nearClip, float farClip)
	{
		m_Left = left;
		m_Right = right;
		m_Bottom = bottom;
		m_Top = top;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		UpdateProjectionMatrix();
	}

	void OrthographicCamera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}