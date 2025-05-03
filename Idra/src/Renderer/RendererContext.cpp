#include "IdraPCH.h"

#include "Renderer/RenderingContext.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Idra {
	RenderingContext* RenderingContext::Create(GLFWwindow* windowHandle)
	{
		//@TODO: Some work will need to be done to refactor this to use a switch on the enum of RendererAPI
		return new OpenGLContext(windowHandle);
	}
}