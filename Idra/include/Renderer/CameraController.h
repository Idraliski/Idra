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

		static CameraController* CreateCameraController(CameraControllerType type);

		virtual void OnUpdate(Camera& camera, Timestep ts) = 0;
		virtual void OnEvent(Camera& camera, Event& e) = 0;
	};
}