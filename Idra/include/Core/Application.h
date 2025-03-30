#pragma once

#include "Core.h"
#include "Window.h"

namespace Idra {

	class IDRA_API Application
	{
	public:
		Application();
		~Application();
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in the client
	Application* CreateApplication();

} // namespace Idra
