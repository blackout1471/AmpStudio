#include "studiopch.h"
#include "Application/Application.h"

int main() {
	auto app = new AmpStudio::Application::Application();
	app->Run();
	app->Close();
	delete app;

	return 0;
}