#include "first_app.hpp"

#include "myve_renderer.hpp"
#include <memory>

namespace myve 
{

	std::unique_ptr<Renderer> renderer;

	void FirstApp::run()
	{
		renderer = std::make_unique<Renderer>();

		renderer->draw();

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

