#pragma once

extern Idra::Application* Idra::CreateApplication();

int main(int argc, char** argv) {
	Idra::Log::Init();

	IDRA_CORE_WARN("Initialized Log!");
	int a = 5;
	IDRA_INFO("Hello! Var={0}", a);

	// Create the application
	auto app = Idra::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
