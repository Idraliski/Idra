#pragma once

#include "Core/Core.h"

namespace Idra {
	class IDRA_API RenderingContext
	{
	public:
		RenderingContext() = default;
		virtual ~RenderingContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		virtual const std::string& GetRenderer() const { return m_Renderer; }
		virtual const std::string& GetVendor() const { return m_Vendor; }
		virtual const std::string& GetVersion() const { return m_Version; }

	protected:
		std::string m_Renderer;
		std::string m_Vendor;
		std::string m_Version;
	};
}