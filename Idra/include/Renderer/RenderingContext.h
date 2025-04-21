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
	};
}