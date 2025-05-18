#pragma once

#include "Core/Core.h"

struct GLFWwindow;

namespace Idra {
	class IDRA_API RenderingContext
	{
	public:
		RenderingContext() = default;
		virtual ~RenderingContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		virtual const std::string& GetRenderer() const = 0;
		virtual const std::string& GetVendor() const = 0;
		virtual const std::string& GetVersion() const = 0;

		static Scope<RenderingContext> Create(GLFWwindow* windowHandle);
	};
}