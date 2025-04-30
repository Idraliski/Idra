#pragma once

#include "Core/Core.h"
#include "Renderer/Camera.h"

namespace Idra {
	class IDRA_API OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float nearClip = 0.1f, float farClip = 1'000.0f);
		~OrthographicCamera() = default;

		void SetProjection(float left, float right, float bottom, float top, float nearClip = 0.1f, float farClip = 1'000.0f);

		const float GetLeft() const { return m_Left; }
		void SetLeft(float left) { m_Left = left; UpdateProjectionMatrix(); }

		const float GetRight() const { return m_Right; }
		void SetRight(float right) { m_Right = right; UpdateProjectionMatrix(); }

		const float GetBottom() const { return m_Bottom; }
		void SetBottom(float bottom) { m_Bottom = bottom; UpdateProjectionMatrix(); }

		const float GetTop() const { return m_Top; }
		void SetTop(float top) { m_Top = top; UpdateProjectionMatrix(); }

	private:
		virtual void UpdateProjectionMatrix() override;
	private:
		float m_Left, m_Right, m_Bottom, m_Top;
	};
}