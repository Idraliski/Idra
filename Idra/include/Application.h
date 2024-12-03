#pragma once

namespace Idra {

	class Application
	{
	public:
		Application();
		~Application();
		void Run();
	};

	// To be defined in the client
	Application* CreateApplication();

} // namespace Idra
