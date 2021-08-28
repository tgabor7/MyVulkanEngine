#include "first_app.hpp"

#include "myve_window.hpp"
#include <memory>

namespace myve 
{

	std::unique_ptr<Window> window;

	void FirstApp::run()
	{
		window = std::make_unique<Window>();

		while (!window->shouldClose()) {


			window->pollEvents();
		}
	}


	void FirstApp::initVulkan()
	{

	}
	void FirstApp::mainLoop()
	{

	}
	void FirstApp::cleanUp()
	{

	}
}

