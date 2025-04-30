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

		IDRA_CORE_INFO("OpenGL Info:");
		IDRA_CORE_INFO("  Vendor: {0}", vendor);
		IDRA_CORE_INFO("  Renderer: {0}", renderer);
		IDRA_CORE_INFO("  Version: {0}", version);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}