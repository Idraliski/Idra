#include "IdraPCH.h"

#include "Renderer/RenderingContext.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Idra {
	RenderingContext* RenderingContext::Create(GLFWwindow* windowHandle)
	{
		return new OpenGLContext(windowHandle);
	}
}