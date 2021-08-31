#include "myve_renderer.hpp"

namespace myve
{
	void Renderer::init()
	{
		const std::vector<VertexData> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};
		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};
		window = std::make_unique<Window>();
		device = std::make_unique<Device>(window->getInstance(), window->getSurface());
		swapchain = std::make_unique<Swapchain>(*device, window->getWindow());
		vbo = std::make_unique<VBO>(*device, vertices, indices);
		pipeline = std::make_unique<Pipeline>(*device, *swapchain, window->getWindow(), *vbo);
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
		vbo->cleanUp();
		swapchain->cleanUp();
		device->cleanUp();
		window->cleanUp();
	}
}