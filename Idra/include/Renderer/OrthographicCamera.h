#pragma once

#include "Core/Core.h"
#include "Renderer/Camera.h"

namespace Idra {
	class IDRA_API OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float nearClip = 0.1f, float farClip = 1'000.0f);
		~OrthographicCamera() = default;

		void SetZoomLevel(float zoomLevel) override;
		void OnResize(float width, float height) override;

		void SetProjection(float aspectRatio, float nearClip = 0.1f, float farClip = 1'000.0f);

		const float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; UpdateProjectionMatrix(); }

	private:
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_AspectRatio;
	};
}