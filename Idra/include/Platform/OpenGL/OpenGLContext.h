#pragma once

#include "IdraPCH.h"

#include "Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Idra {
	class IDRA_API OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext() = default;

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
} // namespace Idra