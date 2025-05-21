#pragma once

#include "Core/Core.h"
#include "Renderer/Camera.h"

namespace Idra {
	class IDRA_API PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov = 45.0f, float aspectRatio = 16.0f/ 9.0f, float nearClip = 0.1f, float farClip = 1'000.f);
		~PerspectiveCamera() = default;

		void SetZoomLevel(float zoomLevel) override;
		void OnResize(float width, float height) override;

		void SetProjection(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 1'000.0f);

		const float GetFOV() const { return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; UpdateProjectionMatrix(); }

		const float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; UpdateProjectionMatrix(); }
	private:
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_FOV;
		float m_AspectRatio;
	};
}