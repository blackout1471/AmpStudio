#include "Application/Application.h"

int main() {
	auto app = new GuitarBlade::Application::Application();
	app->Run();
	app->Close();
	delete app;

	return 0;
}