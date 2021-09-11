#include "myve_renderer.hpp"
#include <unordered_map>
#include "input_handler.hpp"
#include "myve_resource_loader.hpp"

namespace myve
{
	
	void Renderer::init()
	{
		InputHandler::init(window.getWindow());

		
		pipeline->init();
	}
	void Renderer::addGameObject(GameObject object)
	{
		pipeline->addGameObject(object);
	}
	void Renderer::createGameObject()
	{
		GameObject g{device, swapchain};
		g.transformation.scale = glm::vec3{0.01f};
		addGameObject(g);
	}
	Renderer::Renderer()
	{
		//init();
		pipeline = new Pipeline(device, swapchain, window.getWindow());
	}
	Renderer::~Renderer()
	{
	}
	void Renderer::draw()
	{
		window.pollEvents();
		pipeline->drawFrame();
	}
	void Renderer::cleanUp()
	{
		pipeline->cleanUp();
		//vbo->cleanUp();
		swapchain.cleanUp();
		device.cleanUp();
		window.cleanUp();

		delete pipeline;
	}
}