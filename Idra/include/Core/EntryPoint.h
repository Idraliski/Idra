#pragma once

extern Idra::Application* Idra::CreateApplication();

int main(int argc, char** argv) {
	Idra::Log::Init();

	IDRA_CORE_CRITICAL("Welcome! Idra Engine Started");

	// Create the application
	IDRA_PROFILE_BEGIN_SESSION("Startup", "IdraProfile-Startup.json");
	auto app = Idra::CreateApplication();
	IDRA_PROFILE_END_SESSION();

	IDRA_PROFILE_BEGIN_SESSION("Runtime", "IdraProfile-Runtime.json");
	app->Run();
	IDRA_PROFILE_END_SESSION();

	IDRA_PROFILE_BEGIN_SESSION("Shutdown", "IdraProfile-Shutdown.json");
	delete app;
	IDRA_PROFILE_END_SESSION();

	return 0;
}
