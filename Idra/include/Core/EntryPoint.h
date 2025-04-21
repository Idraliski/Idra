#pragma once

extern Idra::Application* Idra::CreateApplication();

int main(int argc, char** argv) {
	Idra::Log::Init();

	IDRA_CORE_CRITICAL("Welcome! Idra Engine Started");

	// Create the application
	auto app = Idra::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
