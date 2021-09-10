#include "myve_renderer.hpp"
#include <unordered_map>
#include "input_handler.hpp"
#include "myve_resource_loader.hpp"

namespace myve
{
	
	void Renderer::init()
	{

		ModelData data = ResourceLoader::loadOBJ("models/bunny.obj");

		

		//texture = std::make_unique<Texture>(device, "textures/bunny-atlas.jpg");

		InputHandler::init(window.getWindow());


		/*vbo = std::make_unique<VBO>(device, data.vertices, data.indices);
		ubo = std::make_unique<UBO>(device, swapchain, *texture);*/

		GameObject room{ device, swapchain, ResourceLoader::loadOBJ("models/viking_room.obj"), new Texture(device, "textures/viking_room.png")};

		pipeline = new Pipeline(device, swapchain, window.getWindow());
		
		for (int i = -2; i < 2; ++i) {
			for (int j = -2; j < 2; ++j) {
				GameObject bunny{ device, swapchain ,ResourceLoader::loadOBJ("models/viking_room.obj"), new Texture(device, "textures/viking_room.png") };
				bunny.transformation.position.z = i * 1;
				bunny.transformation.position.x = j * 1;

				pipeline->addGameObject(bunny);
			}
		}
		

		pipeline->addGameObject(room);

		pipeline->init();
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
		while (!window.shouldClose()) {
			window.pollEvents();
			pipeline->drawFrame();
		}
		cleanUp();
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