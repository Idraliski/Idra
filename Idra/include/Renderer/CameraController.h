#pragma once

#include "Core/Core.h"

#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Renderer/Camera.h"

namespace Idra {
	enum class IDRA_API CameraControllerType 
	{
		None = 0,
		EditorCamera = 1,
		FreeCamera = 2
	};

	class IDRA_API CameraController
	{
	public:
		virtual ~CameraController() = default;

		static Ref<CameraController> CreateCameraController(CameraControllerType type);

		virtual void OnUpdate(Ref<Camera> camera, Timestep ts) = 0;
		virtual void OnEvent(Ref<Camera> camera, Event& e) = 0;
		virtual void OnResize(uint32_t width, uint32_t height, const Ref<Camera>& camera) = 0;
	};
}