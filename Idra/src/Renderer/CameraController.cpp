#include "Renderer/CameraController.h"

#include "Renderer/EditorCameraController.h"

namespace Idra {
    CameraController* Idra::CameraController::CreateCameraController(CameraControllerType type)
    {
		switch (type)
		{
			case CameraControllerType::None:
				IDRA_CORE_ASSERT(false, "CameraControllerType::None is not a valid type!"); // #DEBUG
				return nullptr;
			case CameraControllerType::EditorCamera:
				return new EditorCameraController();
			case CameraControllerType::FreeCamera:
				IDRA_CORE_ASSERT(false, "CameraControllerType::FreeCamera is not implemented yet!"); // #DEBUG
				return nullptr;
		}
        return nullptr;
    }
}

