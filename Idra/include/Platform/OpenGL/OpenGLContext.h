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

		virtual const std::string& GetRenderer() const override { return m_Renderer; }
		virtual const std::string& GetVendor() const override { return m_Vendor; }
		virtual const std::string& GetVersion() const override { return m_Version; }

	private:
		GLFWwindow* m_WindowHandle;

		std::string m_Renderer;
		std::string m_Vendor;
		std::string m_Version;
	};
} // namespace Idra