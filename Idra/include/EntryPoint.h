#pragma once

extern Idra::Application* Idra::CreateApplication();

int main(int argc, char **argv) {
  // Create the application
  auto app = Idra::CreateApplication();

  // Run the application
  app->Run();

  // Delete the application
  delete app;

  return 0;
}
