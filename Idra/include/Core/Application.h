#pragma once

#include "Core.h"

namespace Idra {

	class IDRA_API Application
	{
	public:
		Application();
		~Application();
		void Run();
	};

	// To be defined in the client
	Application* CreateApplication();

} // namespace Idra
