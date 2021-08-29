#include "myve_renderer.hpp"

namespace myve
{
	void Renderer::init()
	{
		window = std::make_unique<Window>();
		device = std::make_unique<Device>(window->getInstance(), window->getSurface());
		swapchain = std::make_unique<Swapchain>(*device, window->getWindow());
		pipeline = std::make_unique<Pipeline>(*device, *swapchain);
	}
	Renderer::Renderer()
	{
		init();
	}
	Renderer::~Renderer()
	{

	}
	void Renderer::draw()
	{
		while (!window->shouldClose()) {
			window->pollEvents();
			pipeline->drawFrame();
		}
	}
	void Renderer::cleanUp()
	{
		pipeline->cleanUp();
		swapchain->cleanUp();
		device->cleanUp();
		window->cleanUp();
	}
}