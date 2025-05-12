#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Idra {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		IDRA_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IDRA_CORE_ASSERT(status, "Failed to initialize Glad!");

		const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

		m_Vendor = vendor ? vendor : "Unknown Vendor";
		m_Renderer = renderer ? renderer : "Unknown Renderer";
		m_Version = version ? version : "Unknown Version";

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}